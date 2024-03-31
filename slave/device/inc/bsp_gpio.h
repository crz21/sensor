#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__    

#define KEY_CT_ADD      GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//色温加 
#define KEY_CT_REDUCE   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//色温减 
#define KEY_DIM_ADD     GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//亮度加
#define KEY_DIM_REDUCE  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//亮度减
#define KEY_TIME_ADD    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)//亮度加
#define KEY_TIME_REDUCE GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//亮度减

#define KEY             (KEY_CT_ADD && KEY_CT_REDUCE && KEY_DIM_ADD && KEY_DIM_REDUCE && KEY_TIME_ADD && KEY_TIME_REDUCE)


#define MOTOR_IN1_OUTPUT      GPIO_WriteBit(GPIOA, GPIO_Pin_0, 1)
#define MOTOR_IN2_OUTPUT      GPIO_WriteBit(GPIOC, GPIO_Pin_15, 1)
#define MOTOR_IN3_OUTPUT      GPIO_WriteBit(GPIOC, GPIO_Pin_14, 1)
#define MOTOR_IN4_OUTPUT      GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1)

#define MOTOR_IN1_STOP_OUTPUT       GPIO_WriteBit(GPIOA, GPIO_Pin_0, 0)
#define MOTOR_IN2_STOP_OUTPUT       GPIO_WriteBit(GPIOC, GPIO_Pin_15, 0)
#define MOTOR_IN3_STOP_OUTPUT       GPIO_WriteBit(GPIOC, GPIO_Pin_14, 0)
#define MOTOR_IN4_STOP_OUTPUT       GPIO_WriteBit(GPIOC, GPIO_Pin_14, 0)

#define MOTOR_1     PAout(0)
#define MOTOR_2     PCout(15)
#define MOTOR_3     PCout(14)
#define MOTOR_4     PCout(13)

void hal_key_init(void);
void bsp_stepping_motor_init(void);
#endif

