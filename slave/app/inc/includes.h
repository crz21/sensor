#ifndef __INCLUDES_H__
#define __INCLUDES_H__

//库头文件
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "stm32f10x_conf.h"
#if RTT_FUNC
#include "SEGGER_RTT.h"
#endif

//外设头文件
#include "bsp_i2c.h"
#include "bsp_gpio.h"
#include "bsp_sys.h"
#include "bsp_uart.h"
#include "bsp_spi.h"

//驱动头文件
#include "sys.h"
#include "wireless.h"
#include "usart.h"

#endif
