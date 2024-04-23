#include "includes.h"

void bsp_i2c_init(void)
{
    GPIO_InitTypeDef GPIO_Init;

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_Init.Pin = GPIO_PIN_6;
    GPIO_Init.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_Init);

    GPIO_Init.Pin = GPIO_PIN_7;
    GPIO_Init.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_Init);

    IIC_SCL_H;
    IIC_SDA_H;
}

// 产生IIC起始信号
void IIC_Start(void)
{
    IIC_SDA_H;
    IIC_SCL_H;
    delay_us(4);
    IIC_SDA_L; // START:when CLK is high,DATA change form high to low
    delay_us(4);
    IIC_SCL_L; // 钳住I2C总线，准备发送或接收数据
}
// 产生IIC停止信号
void IIC_Stop(void)
{
    IIC_SCL_L;
    IIC_SDA_L; // STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC_SCL_H;
    IIC_SDA_H; // 发送I2C总线结束信号
    delay_us(4);
}
// 等待应答信号到来
// 返回值：1，接收应答失败
//         0，接收应答成功
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;

    IIC_SDA_H;
    delay_us(1);
    IIC_SCL_H;
    delay_us(1);
    while (READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_L; // 时钟输出0
    return 0;
}
// 产生ACK应答
void IIC_Ack(uint8_t ack)
{
    IIC_SCL_L;

    IIC_SDA = (ack) ? 1 : 0;

    delay_us(2);
    IIC_SCL_H;
    delay_us(2);
    IIC_SCL_L;
}

// IIC发送一个字节
// 返回从机有无应答
// 1，有应答
// 0，无应答
void IIC_Send_Byte(u8 dev_n, u8 txd)
{
    u8 t;

    IIC_SCL_L; // 拉低时钟开始数据传输

    for (t = 0; t < 8; t++)
    {
        IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1;

        delay_us(2); // 对TEA5767这三个延时都是必须的
        IIC_SCL_H;
        delay_us(2);
        IIC_SCL_L;
        delay_us(2);
    }

    if (DEV_OLED == dev_n)
    {
        IIC_SDA_H;
        delay_us(1);
        IIC_SCL_H;

        delay_us(1);
        IIC_SCL_L;
        delay_us(1);
    }
}
// 读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;

    for (i = 0; i < 8; i++)
    {
        IIC_SCL_L;
        delay_us(2);
        IIC_SCL_H;
        receive <<= 1;
        if (READ_SDA)
        {
            receive++;
        }
        delay_us(1);
    }

    IIC_Ack(ack);

    return receive;
}
