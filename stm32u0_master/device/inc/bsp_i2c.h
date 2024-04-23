#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#include <stdint.h>

#define IIC_SCL_H HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
#define IIC_SCL_L HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
#define IIC_SDA_H HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
#define IIC_SDA_L HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
#define READ_SDA HAL_GPIO_ReadPin(cGPIOB, GPIO_PIN_7);
typedef enum
{
    DEV_OLED = 0,
    DEV_EPROM,
} DEV_NUM;
// IIC所有操作函数
void hal_i2c_init(void);              // 初始化IIC的IO口
void IIC_Start(void);                 // 发送IIC开始信号
void IIC_Stop(void);                  // 发送IIC停止信号
void IIC_Send_Byte(u8 dev_n, u8 txd); // IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);  // IIC读取一个字节
u8 IIC_Wait_Ack(void);                // IIC等待ACK信号
void IIC_Ack(uint8_t ack);            // IIC发送ACK信号
// void IIC_NAck(void);                //IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC_Read_One_Byte(u8 daddr, u8 addr);
#endif
