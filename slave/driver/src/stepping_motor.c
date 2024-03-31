
#include "includes.h"

//顺时针转动
static void MotorCW(void) 
{
    MOTOR_IN1_OUTPUT;
    delay_ms(MOTOR_SPEED);
    MOTOR_IN2_OUTPUT;
    delay_ms(MOTOR_SPEED);
    MOTOR_IN3_OUTPUT;
    delay_ms(MOTOR_SPEED);
    MOTOR_IN4_OUTPUT;
    delay_ms(MOTOR_SPEED);

//  MOTOR_1 = 1;
//  delay_ms(MOTOR_SPEED);
//  MOTOR_2 = 1;
//  delay_ms(MOTOR_SPEED);
//  MOTOR_3 = 1;
//  delay_ms(MOTOR_SPEED);
//  MOTOR_4 = 1;
//  delay_ms(MOTOR_SPEED);
}

//逆时针转动
static void MotorCCW(void) 
{
    MOTOR_IN4_OUTPUT;
    delay_ms(MOTOR_SPEED);
    MOTOR_IN3_OUTPUT;
    delay_ms(MOTOR_SPEED);
    MOTOR_IN2_OUTPUT;
    delay_ms(MOTOR_SPEED);
    MOTOR_IN1_OUTPUT;
    delay_ms(MOTOR_SPEED);

//  MOTOR_4 = 1;
//  delay_ms(MOTOR_SPEED);
//  MOTOR_3 = 1;
//  delay_ms(MOTOR_SPEED);
//  MOTOR_2 = 1;
//  delay_ms(MOTOR_SPEED);
//  MOTOR_1 = 1;
//  delay_ms(MOTOR_SPEED);
}

//停止转动
static void MotorStop(void)
{
//    MOTOR_1 = 0;
//    MOTOR_2 = 0;
//    MOTOR_3 = 0;
//    MOTOR_4 = 0;
MOTOR_IN1_STOP_OUTPUT;
MOTOR_IN2_STOP_OUTPUT;
MOTOR_IN3_STOP_OUTPUT;
MOTOR_IN4_STOP_OUTPUT;
}

void stepping_motor_init(void)
{
    bsp_stepping_motor_init();
}

void stepping_motor_task(void)
{
    uint16_t i;

    for (i = 0; i < 500; i++) 
    {
        MotorCW();   //顺时针转动
    }

    MotorStop();  //停止转动

    delay_ms(500);

    IWDG_ReloadCounter();

    for (i = 0; i < 500; i++) 
    {
        MotorCCW();  //逆时针转动
    }

    MotorStop();  //停止转动

    delay_ms(500);
}

