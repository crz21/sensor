#include "includes.h"

//Mini STM32������
//24CXX��������(�ʺ�24C01~24C16,24C32~256δ��������!�д���֤!)
//����ԭ��@ALIENTEK
//2010/6/10
//V1.2

//��ʼ��IIC�ӿ�
void eeprom_init(void)
{
    hal_i2c_init();
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{                 
    u8 temp=0;                                                                               
    IIC_Start();  
    if(EE_TYPE>AT24C16)
    {
        IIC_Send_Byte(DEV_EPROM, 0XA0);    //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(DEV_EPROM, ReadAddr>>8);//���͸ߵ�ַ
//      IIC_Wait_Ack();      
    }
    else 
        IIC_Send_Byte(DEV_EPROM, 0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д����      

    IIC_Wait_Ack(); 
  IIC_Send_Byte(DEV_EPROM, ReadAddr%256);   //���͵͵�ַ
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
    IIC_Stop();//����һ��ֹͣ����       
    return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{                                                                                            
    IIC_Start();  
    if(EE_TYPE>AT24C16)
    {
        IIC_Send_Byte(DEV_EPROM, 0XA0);     //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(DEV_EPROM, WriteAddr>>8);//���͸ߵ�ַ
    }
    else
    {
        IIC_Send_Byte(DEV_EPROM, 0XA0+((WriteAddr/256)<<1));   //����������ַ0XA1,д���� 
    }    
    IIC_Wait_Ack();    
  IIC_Send_Byte(DEV_EPROM, WriteAddr%256);   //���͵͵�ַ
    IIC_Wait_Ack();                                                        
    IIC_Send_Byte(DEV_EPROM, DataToWrite);     //�����ֽ�                              
    IIC_Wait_Ack();                    
  IIC_Stop();//����һ��ֹͣ���� 
    delay_ms(10);    
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{   
    u8 t;
    for(t=0;t<Len;t++)
    {
        AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
    }                                                   
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
    u8 temp;
    
    temp=AT24CXX_ReadOneByte(EE_TYPE);//����ÿ�ο�����дAT24CXX            
    if(temp==0X55)
    {
        return 0;          
    }
    else//�ų���һ�γ�ʼ�������
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

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
    while(NumToRead)
    {
        *pBuffer++=AT24CXX_ReadOneByte(ReadAddr++); 
        NumToRead--;
    }
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
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

    //��ȡ�����洢��־
    flag = AT24CXX_ReadOneByte(FLAG_ADDRESS);

    if (flag == 0x55) 
    {

            //��ȡɫ��
            exp_ct_value = AT24CXX_ReadOneByte(CT_GRADE_ADDRESS);

            //��ȡ����
            exp_bright_value = AT24CXX_ReadOneByte(BRIGHT_GRADE_ADDRESS);

            exp_ct_value = 127;
            exp_bright_value = 255;
    }  
    else 
    {
        save_fun = 1;
        AT24CXX_WriteOneByte(SAVE_FUN, save_fun);

        //д��ɫ��
        exp_ct_value = 127;
        AT24CXX_WriteOneByte(CT_GRADE_ADDRESS, exp_ct_value);

        //д������
        exp_bright_value = 255;
        AT24CXX_WriteOneByte(BRIGHT_GRADE_ADDRESS, exp_bright_value);

        AT24CXX_WriteOneByte(FLAG_ADDRESS, 0x55);
    }
}











