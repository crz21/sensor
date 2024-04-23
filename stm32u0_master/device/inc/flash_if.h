#ifndef __FLASH_IF_H
#define __FLASH_IF_H

#include <stdint.h>

//#if defined(STM32F030)
//// CCT6
//// #define PAGE_SIZE                         (0x800)    /* 2 Kbyte */
//// #define FLASH_SIZE                        (0x40000)  /* 64 KBytes */
//// C8T6
#define PAGE_SIZE (0x400)    /* 1 Kbyte */
#define FLASH_SIZE (0x10000) /* 64 KBytes */
//#else
//#error "Please select first the STM32 device to be used (in stm32f0xx.h)"
//#endif

#define Flash_If_Log(x)

#define BOOTADDRESS (0x08000000)
#define UPGRADEADDRESS (0x08000000) // 16kB
#define RDM_UPGRADE_SIZE (0x80)     // 128B
#define FLASH_SAVE_DATA_ADDR (0x0800FC00)
/* Compute the FLASH image size  */
#define FLASH_IMAGE_SIZE (uint32_t)(FLASH_SIZE - (UPGRADEADDRESS - BOOTADDRESS))

uint8_t Flash_PageErase(uint32_t size);
uint32_t Flash_Write(uint32_t addr, uint8_t *data, uint32_t size);
uint32_t Flash_Read(uint32_t addr, uint8_t *data, uint32_t size);
void Flash_StartAddrSet(uint32_t addr);

#endif
