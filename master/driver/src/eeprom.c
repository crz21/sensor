#include "includes.h"

//Mini STM32开发板
//24CXX驱动函数(适合24C01~24C16,24C32~256未经过测试!有待验证!)
//正点原子@ALIENTEK
//2010/6/10
//V1.2

//初始化IIC接口
void eeprom_init(void)
{
    hal_i2c_init();
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{                 
    u8 temp=0;                                                                               
    IIC_Start();  
    if(EE_TYPE>AT24C16)
    {
        IIC_Send_Byte(DEV_EPROM, 0XA0);    //发送写命令
        IIC_Wait_Ack();
        IIC_Send_Byte(DEV_EPROM, ReadAddr>>8);//发送高地址
//      IIC_Wait_Ack();      
    }
    else 
        IIC_Send_Byte(DEV_EPROM, 0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据      

    IIC_Wait_Ack(); 
  IIC_Send_Byte(DEV_EPROM, ReadAddr%256);   //发送低地址
    IIC_Wait_Ack();     
    IIC_Start();
    if(EE_TYPE > AT24C16)   
    {
        IIC_Send_Byte(DEV_EPROM, 0XA1);
    }
    else
    {
        IIC_Send_Byte(DEV_EPROM, 0XA1+((ReadAddr/256)<<1));
    }
       
    IIC_Wait_Ack();  
    temp=IIC_Read_Byte(0);         
    IIC_Stop();//产生一个停止条件       
    return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{                                                                                            
    IIC_Start();  
    if(EE_TYPE>AT24C16)
    {
        IIC_Send_Byte(DEV_EPROM, 0XA0);     //发送写命令
        IIC_Wait_Ack();
        IIC_Send_Byte(DEV_EPROM, WriteAddr>>8);//发送高地址
    }
    else
    {
        IIC_Send_Byte(DEV_EPROM, 0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA1,写数据 
    }    
    IIC_Wait_Ack();    
  IIC_Send_Byte(DEV_EPROM, WriteAddr%256);   //发送低地址
    IIC_Wait_Ack();                                                        
    IIC_Send_Byte(DEV_EPROM, DataToWrite);     //发送字节                              
    IIC_Wait_Ack();                    
  IIC_Stop();//产生一个停止条件 
    delay_ms(10);    
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{   
    u8 t;
    for(t=0;t<Len;t++)
    {
        AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
    }                                                   
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{   
    u8 t;
    u32 temp=0;
    for(t=0;t<Len;t++)
    {
        temp<<=8;
        temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1);                       
    }
    return temp;                                                    
}
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
    u8 temp;
    
    temp=AT24CXX_ReadOneByte(EE_TYPE);//避免每次开机都写AT24CXX            
    if(temp==0X55)
    {
        return 0;          
    }
    else//排除第一次初始化的情况
    {
        AT24CXX_WriteOneByte(EE_TYPE,0X55);
      temp=AT24CXX_ReadOneByte(EE_TYPE);
      
        if(temp==0X55)
        {
            return 0;
        }
    }
    return 1;                                             
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
    while(NumToRead)
    {
        *pBuffer++=AT24CXX_ReadOneByte(ReadAddr++); 
        NumToRead--;
    }
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
    while(NumToWrite--)
    {
        AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}
 
void read_all_data_from_eeprom(void)
{
    uint8_t flag = 0;

    //读取参数存储标志
    flag = AT24CXX_ReadOneByte(FLAG_ADDRESS);

    if (flag == 0x55) 
    {

            //读取色温
            exp_ct_value = AT24CXX_ReadOneByte(CT_GRADE_ADDRESS);

            //读取亮度
            exp_bright_value = AT24CXX_ReadOneByte(BRIGHT_GRADE_ADDRESS);

            exp_ct_value = 127;
            exp_bright_value = 255;
    }  
    else 
    {
        save_fun = 1;
        AT24CXX_WriteOneByte(SAVE_FUN, save_fun);

        //写入色温
        exp_ct_value = 127;
        AT24CXX_WriteOneByte(CT_GRADE_ADDRESS, exp_ct_value);

        //写入亮度
        exp_bright_value = 255;
        AT24CXX_WriteOneByte(BRIGHT_GRADE_ADDRESS, exp_bright_value);

        AT24CXX_WriteOneByte(FLAG_ADDRESS, 0x55);
    }
}











