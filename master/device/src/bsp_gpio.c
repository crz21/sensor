#include "includes.h"

//������ʼ������ 
void hal_key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTA,PORTCʱ��
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//PA2 CT+
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA2
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//PA3 CT-
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA3
 
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//PA4 DIM+
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������    
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA4

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PA5 DIM-
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������    
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA5

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PB1 TIME+
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA5

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;//PB0 TIME-
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������    
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA5
} 

//led��ʼ������
void hal_led_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
    
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);  //ʹ��PA,PD�˿�ʱ��
    
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;               //LED0-->PA.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);                  //�����趨������ʼ��GPIOA.8
 GPIO_SetBits(GPIOA,GPIO_Pin_8);                         //PA.8 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;               //LED1-->PD.2 �˿�����, �������
 GPIO_Init(GPIOD, &GPIO_InitStructure);                  //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOD,GPIO_Pin_2);                         //PD.2 ����� 
}

/* �������IO��ʼ�� */
void bsp_stepping_motor_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
    
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��PA,PC�˿�ʱ��

 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //IO���ٶ�Ϊ50MHz   

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;               //IN1-->PA.0 �˿�����
 GPIO_Init(GPIOA, &GPIO_InitStructure);                  //�����趨������ʼ��GPIOA.0
// MOTOR_IN1_SET_HIGH;                                     //PA.0 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;              //IN2-->PC.15 �˿�����
 GPIO_Init(GPIOC, &GPIO_InitStructure);                  //�����趨������ʼ��GPIOC.15
// MOTOR_IN2_SET_HIGH;                                     //PC.15 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;              //IN3-->PC.14 �˿�����
 GPIO_Init(GPIOC, &GPIO_InitStructure);                  //�����趨������ʼ��GPIOA.8
// MOTOR_IN3_SET_HIGH;                                     //PC.14 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;              //IN4-->PC.13 �˿�����, �������
 GPIO_Init(GPIOC, &GPIO_InitStructure);                  //������� ��IO���ٶ�Ϊ50MHz
// MOTOR_IN4_SET_HIGH;                                     //PC.13 �����
}

