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

// ����IIC��ʼ�ź�
void IIC_Start(void)
{
    IIC_SDA_H;
    IIC_SCL_H;
    delay_us(4);
    IIC_SDA_L; // START:when CLK is high,DATA change form high to low
    delay_us(4);
    IIC_SCL_L; // ǯסI2C���ߣ�׼�����ͻ��������
}
// ����IICֹͣ�ź�
void IIC_Stop(void)
{
    IIC_SCL_L;
    IIC_SDA_L; // STOP:when CLK is high DATA change form low to high
    delay_us(4);
    IIC_SCL_H;
    IIC_SDA_H; // ����I2C���߽����ź�
    delay_us(4);
}
// �ȴ�Ӧ���źŵ���
// ����ֵ��1������Ӧ��ʧ��
//         0������Ӧ��ɹ�
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
    IIC_SCL_L; // ʱ�����0
    return 0;
}
// ����ACKӦ��
void IIC_Ack(uint8_t ack)
{
    IIC_SCL_L;

    IIC_SDA = (ack) ? 1 : 0;

    delay_us(2);
    IIC_SCL_H;
    delay_us(2);
    IIC_SCL_L;
}

// IIC����һ���ֽ�
// ���شӻ�����Ӧ��
// 1����Ӧ��
// 0����Ӧ��
void IIC_Send_Byte(u8 dev_n, u8 txd)
{
    u8 t;

    IIC_SCL_L; // ����ʱ�ӿ�ʼ���ݴ���

    for (t = 0; t < 8; t++)
    {
        IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1;

        delay_us(2); // ��TEA5767��������ʱ���Ǳ����
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
// ��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
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
