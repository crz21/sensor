#include "includes.h"

//按键初始化函数 
void hal_key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA,PORTC时钟
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//PA2 CT+
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA2
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//PA3 CT-
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA3
 
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//PA4 DIM+
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入    
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA4

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PA5 DIM-
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入    
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA5

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PB1 TIME+
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA5

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PB0 TIME-
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA5
} 

//led初始化函数
void hal_led_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
    
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);  //使能PA,PD端口时钟
    
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;               //LED0-->PA.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);                  //根据设定参数初始化GPIOA.8
 GPIO_SetBits(GPIOA,GPIO_Pin_8);                         //PA.8 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;               //LED1-->PD.2 端口配置, 推挽输出
 GPIO_Init(GPIOD, &GPIO_InitStructure);                  //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOD,GPIO_Pin_2);                         //PD.2 输出高 
}

/* 步进电机IO初始化 */
void bsp_stepping_motor_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
    
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);  //使能PA,PC端口时钟

 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //IO口速度为50MHz   

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;               //IN1-->PA.0 端口配置
 GPIO_Init(GPIOA, &GPIO_InitStructure);                  //根据设定参数初始化GPIOA.0
// MOTOR_IN1_SET_HIGH;                                     //PA.0 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;              //IN2-->PC.15 端口配置
 GPIO_Init(GPIOC, &GPIO_InitStructure);                  //根据设定参数初始化GPIOC.15
// MOTOR_IN2_SET_HIGH;                                     //PC.15 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;              //IN3-->PC.14 端口配置
 GPIO_Init(GPIOC, &GPIO_InitStructure);                  //根据设定参数初始化GPIOA.8
// MOTOR_IN3_SET_HIGH;                                     //PC.14 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;              //IN4-->PC.13 端口配置, 推挽输出
 GPIO_Init(GPIOC, &GPIO_InitStructure);                  //推挽输出 ，IO口速度为50MHz
// MOTOR_IN4_SET_HIGH;                                     //PC.13 输出高
}

