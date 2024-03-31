#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#define KEY_ENTER (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)) // 色温加
#define KEY_UP (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))    // 色温减
#define KEY_DOWN (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))  // 亮度加
#define KEY_MENU (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5))  // 亮度减

#define KEY (KEY_ENTER && KEY_UP && KEY_DOWN && KEY_MENU)

#define MOTOR_IN1_OUTPUT GPIO_WriteBit(GPIOA, GPIO_Pin_0, 1)
#define MOTOR_IN2_OUTPUT GPIO_WriteBit(GPIOC, GPIO_Pin_15, 1)
#define MOTOR_IN3_OUTPUT GPIO_WriteBit(GPIOC, GPIO_Pin_14, 1)
#define MOTOR_IN4_OUTPUT GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1)

#define MOTOR_IN1_STOP_OUTPUT GPIO_WriteBit(GPIOA, GPIO_Pin_0, 0)
#define MOTOR_IN2_STOP_OUTPUT GPIO_WriteBit(GPIOC, GPIO_Pin_15, 0)
#define MOTOR_IN3_STOP_OUTPUT GPIO_WriteBit(GPIOC, GPIO_Pin_14, 0)
#define MOTOR_IN4_STOP_OUTPUT GPIO_WriteBit(GPIOC, GPIO_Pin_14, 0)

#define MOTOR_1 PAout(0)
#define MOTOR_2 PCout(15)
#define MOTOR_3 PCout(14)
#define MOTOR_4 PCout(13)

void hal_key_init(void);
void bsp_stepping_motor_init(void);
#endif
