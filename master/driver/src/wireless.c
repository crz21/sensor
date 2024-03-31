#include "includes.h"

#define TX_ADR_WIDTH (5)    // 5字节地址宽度
#define RX_ADR_WIDTH (5)    // 5字节地址宽度
#define TX_PLOAD_WIDTH (10) // 32字节有效数据宽度
#define RX_PLOAD_WIDTH (10) // 32字节有效数据宽度
#define ADJUST_VALUE (30)

const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t test_buf[32] = {5, 0x12, 0x22, 0x33, 0x44, 0x55};
uint8_t test_get_buf[30];
uint8_t wireless_head[2] = {0x5A, 0xA4};
uint8_t wireless_tail[2] = {0x0D, 0x0E};

void read_bit(void)
{
}

uint8_t spi_swapbyte(uint8_t byte)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        ((uint8_t)(byte & 0x80) == 0x80) ? NRF24L01_MOSI_H : NRF24L01_MOSI_L;

        NRF24L01_SCK_H;

        byte = (byte << 1);
        byte |= NRF24L01_MISO_READ;

        NRF24L01_SCK_L;
    }

    return byte;
}

uint8_t nrf24l01_read_reg(uint8_t reg)
{
    uint8_t value;

    NRF24L01_CSN_L;
    spi_swapbyte(reg);
    value = spi_swapbyte(NOP);
    NRF24L01_CSN_H;

    return value;
}

uint8_t nrf24l01_read_buf(uint8_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t status, i;

    NRF24L01_CSN_L;
    status = spi_swapbyte(reg);
    for (i = 0; i < len; i++)
    {
        buf[i] = spi_swapbyte(NOP);
    }
    NRF24L01_CSN_H;
    return status;
}

uint8_t nrf24l01_write_reg(uint8_t reg, uint8_t value)
{
    uint8_t status;

    NRF24L01_CSN_L;
    status = spi_swapbyte(reg);
    spi_swapbyte(value);
    NRF24L01_CSN_H;

    return status;
}

uint8_t nrf24l01_write_buf(uint8_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t status, i;
    NRF24L01_CSN_L;
    status = spi_swapbyte(reg);
    for (i = 0; i < len; i++)
    {
        spi_swapbyte(*buf++);
    }
    NRF24L01_CSN_H;
    return status;
}

uint8_t nrf24l01_get_rxbuf(uint8_t *buf)
{
    uint8_t state;
    state = nrf24l01_read_reg(STATUS);
    nrf24l01_write_reg(nRF_WRITE_REG + STATUS, state);
    if (state & RX_OK)
    {
        NRF24L01_CE_H;
        nrf24l01_read_buf(RD_RX_PLOAD, buf, RX_PLOAD_WIDTH);
        nrf24l01_write_reg(FLUSH_RX, NOP);
        NRF24L01_CE_H;
        delay_us(150);
        return 0;
    }
    return 1;
}

uint8_t nrf24l01_send_txbuf(uint8_t *buf)
{
    uint8_t state;

    NRF24L01_CE_L;
    nrf24l01_write_buf(WR_TX_PLOAD, buf, TX_PLOAD_WIDTH);
    NRF24L01_CE_H;
    while (NRF24L01_IRQ_READ == 1)
        ;
    state = nrf24l01_read_reg(STATUS);
    nrf24l01_write_reg(nRF_WRITE_REG + STATUS, state);

    if (state & MAX_TX)
    {
        nrf24l01_write_reg(FLUSH_TX, NOP);
        return MAX_TX;
    }

    if (state & TX_OK)
    {
        return TX_OK;
    }
    return NOP;
}

uint8_t nrf24l01_check(void)
{
    uint8_t check_in_buf[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
    uint8_t check_out_buf[5] = {0x00};
    uint8_t i = 0;

    NRF24L01_SCK_L;
    NRF24L01_CSN_H;
    NRF24L01_CE_L;

    nrf24l01_write_buf(nRF_WRITE_REG + TX_ADDR, check_in_buf, 5);
    nrf24l01_read_buf(nRF_READ_REG + TX_ADDR, check_out_buf, 5);

    i = ((check_out_buf[0] == 0x11) &&
         (check_out_buf[1] == 0x22) &&
         (check_out_buf[2] == 0x33) &&
         (check_out_buf[3] == 0x44) &&
         (check_out_buf[4] == 0x55))
            ? 0
            : 1;
    return i;
}

void nrf24l01_rt_init(void)
{
    NRF24L01_CE_L;
    nrf24l01_write_reg(nRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);
    nrf24l01_write_reg(FLUSH_RX, NOP);
    nrf24l01_write_buf(nRF_WRITE_REG + TX_ADDR, (uint8_t *)TX_ADDRESS, TX_ADR_WIDTH);
    nrf24l01_write_buf(nRF_WRITE_REG + RX_ADDR_P0, (uint8_t *)RX_ADDRESS, RX_ADR_WIDTH);
    nrf24l01_write_reg(nRF_WRITE_REG + EN_AA, 0x01);
    nrf24l01_write_reg(nRF_WRITE_REG + EN_RXADDR, 0x01);
    nrf24l01_write_reg(nRF_WRITE_REG + SETUP_RETR, 0x1A);
    nrf24l01_write_reg(nRF_WRITE_REG + RF_CH, 0);
    nrf24l01_write_reg(nRF_WRITE_REG + RF_SETUP, 0x0F);
    nrf24l01_write_reg(nRF_WRITE_REG + CONFIG, 0x0F);
    NRF24L01_CE_H;
}

void wireless_init()
{
    wireless_pin_init();
    while (nrf24l01_check())
        ;
    nrf24l01_rt_init();
}

void nrf24l01_sendbuf(uint8_t *buf)
{
    NRF24L01_CE_L;
    nrf24l01_write_reg(nRF_WRITE_REG + CONFIG, 0x0E);
    NRF24L01_CE_H;
    delay_us(15);
    nrf24l01_send_txbuf(buf);
    NRF24L01_CE_L;
    nrf24l01_write_reg(nRF_WRITE_REG + CONFIG, 0x0F);
    NRF24L01_CE_H;
}

uint8_t nrf24l01_get_value_flag()
{
    return NRF24L01_IRQ_READ;
}

void wireless_task(void)
{
    if (sys_tim_param.sys_tim_500ms_flag)
    {
        sys_tim_param.sys_tim_500ms_flag = 0;
        if (nrf24l01_get_value_flag() == 0)
        {
            nrf24l01_get_rxbuf(test_get_buf);

            if (0 == memcmp(test_get_buf, wireless_head, 2) && 0 == memcmp(test_get_buf + 7, wireless_tail, 2) && 1 == current_index)
            {
                key_on = 1;
            }
        }
    }
}
