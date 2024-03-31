#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__
 
#include <stdint.h>
               
//IO��������
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X80000000;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X30000000;}

//IO��������     
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA    
#define READ_SDA   PBin(7)  //����SDA 
typedef enum{
    DEV_OLED = 0,
    DEV_EPROM,
}DEV_NUM;
//IIC���в�������
void hal_i2c_init(void);                //��ʼ��IIC��IO��                
void IIC_Start(void);               //����IIC��ʼ�ź�
void IIC_Stop(void);                //����IICֹͣ�ź�
void IIC_Send_Byte(u8 dev_n, u8 txd);           //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void);              //IIC�ȴ�ACK�ź�
void IIC_Ack(uint8_t ack);                 //IIC����ACK�ź�
// void IIC_NAck(void);                //IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);   
#endif
















