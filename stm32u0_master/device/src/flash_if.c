#include "includes.h"

// flash操作的起始地址设置
const static uint32_t flash_addr = FLASH_SAVE_DATA_ADDR;

uint8_t FLASH_CheckWriteProtectionPages(uint32_t *mask)
{
  uint32_t BlockNbr = 0, UserFlashMask = 0;
  uint8_t ret;

  if (flash_addr < 0x08000000)
  {
    while (1)
      ;
  }

  FLASH_Unlock();

  /* Get the number of block (4 or 2 pages) from where the user program will be loaded */
  BlockNbr = (flash_addr - 0x08000000) >> 12;

  /* Compute the mask to test if the Flash memory, where the user program will be
     loaded, is write protected */
#if defined(STM32F10X_MD) || defined(STM32F10X_MD_VL)
  UserFlashMask = ((uint32_t) ~((1 << BlockNbr) - 1));
#else
  if (BlockNbr < 62)
  {
    UserFlashMask = ((uint32_t) ~((1 << BlockNbr) - 1));
  }
  else
  {
    UserFlashMask = ((uint32_t)0x80000000);
  }
#endif

  *mask = UserFlashMask;

  /* Test if any page of Flash memory where program user will be loaded is write protected */
  if ((FLASH_GetWriteProtectionOptionByte() & UserFlashMask) != UserFlashMask)
  {
    ret = 1;
    Flash_If_Log("Flash memory is write protected\r\n");
  }
  else
  {
    ret = 0;
  }

  return ret;
}

void FLASH_DisableWriteProtectionPages(uint32_t mask)
{
  uint32_t useroptionbyte = 0, WRPR = 0;
  uint16_t var1 = OB_IWDG_SW, var2 = OB_STOP_NoRST, var3 = OB_STDBY_NoRST;
  FLASH_Status status = FLASH_BUSY;

  if (flash_addr < 0x08000000)
  {
    while (1)
      ;
  }

  WRPR = FLASH_GetWriteProtectionOptionByte();

  /* Test if user memory is write protected */
  if ((WRPR & mask) != mask)
  {
    useroptionbyte = FLASH_GetUserOptionByte();

    mask |= WRPR;

    status = FLASH_EraseOptionBytes();

    if (mask != 0xFFFFFFFF)
    {
      status = FLASH_EnableWriteProtection((uint32_t)~mask);
    }

    /* Test if user Option Bytes are programmed */
    if ((useroptionbyte & 0x07) != 0x07)
    {
      /* Restore user Option Bytes */
      if ((useroptionbyte & 0x01) == 0x0)
      {
        var1 = OB_IWDG_HW;
      }
      if ((useroptionbyte & 0x02) == 0x0)
      {
        var2 = OB_STOP_RST;
      }
      if ((useroptionbyte & 0x04) == 0x0)
      {
        var3 = OB_STDBY_RST;
      }

      FLASH_UserOptionByteConfig(var1, var2, var3);
    }

    if (status == FLASH_COMPLETE)
    {
      Flash_If_Log("Write Protection disabled...\r\n");

      Flash_If_Log("...and a System Reset will be generated to re-load the new option bytes\r\n");

      /* Generate System Reset to load the new option byte values */
      NVIC_SystemReset();
    }
    else
    {
      Flash_If_Log("Error: Flash write unprotection failed...\r\n");
    }
  }
  else
  {
    Flash_If_Log("Flash memory not write protected\r\n");
  }
}

uint32_t FLASH_PagesMask(uint32_t size)
{
  uint32_t pagenumber = 0x0;

  if (flash_addr < 0x08000000)
  {
    while (1)
      ;
  }

  if ((size % PAGE_SIZE) != 0)
  {
    pagenumber = (size / PAGE_SIZE) + 1;
  }
  else
  {
    pagenumber = size / PAGE_SIZE;
  }
  return pagenumber;
}

void Flash_PageProtect(void)
{
  uint32_t mask = 0;

  if (1 == FLASH_CheckWriteProtectionPages(&mask))
  {
    FLASH_DisableWriteProtectionPages(mask);
  }
}

uint8_t Flash_PageErase(uint32_t size)
{
  uint32_t NbrOfPage = 0;
  uint32_t EraseCounter = 0x0;
  uint32_t mask = 0;
  FLASH_Status FLASHStatus = FLASH_COMPLETE;
  uint8_t ret;

  if (flash_addr < 0x08000000)
  {
    while (1)
      ;
  }

  if (size > FLASH_IMAGE_SIZE)
    return 1;

  if (1 == FLASH_CheckWriteProtectionPages(&mask))
  {
    FLASH_DisableWriteProtectionPages(mask);
  }

  /* Erase the needed pages where the user application will be loaded */
  /* Define the number of page to be erased */
  NbrOfPage = FLASH_PagesMask(size);

  /* Erase the FLASH pages */
  for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
  {
    FLASHStatus = FLASH_ErasePage(flash_addr + (PAGE_SIZE * EraseCounter));
  }

  if (EraseCounter != NbrOfPage)
  {
    ret = 1;
    Flash_If_Log("Error: Flash erase failed...\r\n");
  }
  else
  {
    ret = 0;
  }

  return ret;
}

uint32_t Flash_Write(uint32_t addr, uint8_t *data, uint32_t size)
{
  uint32_t RamSource;
  uint32_t j = 0;
  FLASH_Status FLASHStatus = FLASH_COMPLETE;

  if (flash_addr < 0x08000000)
  {
    while (1)
      ;
  }

  if (size > FLASH_IMAGE_SIZE)
    return 0;

  //	RamSource = (uint32_t)data;
  FLASH_Unlock();
  FLASHStatus = FLASH_ErasePage(addr);
  if (FLASH_COMPLETE != FLASHStatus)
  {
    return FLASHStatus;
  }
  for (j = 0; j < size; j += 4)
  {
    memcpy(&RamSource, &data[j], sizeof(RamSource));

    /* Program the data received into STM32F10x Flash */
    FLASHStatus = FLASH_ProgramWord(addr, RamSource);

    if ((*(uint32_t *)addr != RamSource) || (FLASHStatus != FLASH_COMPLETE))
    {
      j = 0;
      Flash_If_Log("Error: Flash program failed...\r\n");
      break;
    }
    addr += 4;
    //		RamSource += 4;
  }

  FLASH_Lock();

  return j;
}

uint32_t Flash_Read(uint32_t addr, uint8_t *data, uint32_t size)
{
  uint32_t RamSource;
  uint32_t j = 0;

  if (flash_addr < 0x08000000)
  {
    while (1)
      ;
  }

  if (size > FLASH_IMAGE_SIZE)
    return 0;

  for (j = 0; j < size; j += 4)
  {
    RamSource = *(uint32_t *)addr;
    memcpy(&data[j], &RamSource, 4);

    addr += 4;
  }

  return j;
}
