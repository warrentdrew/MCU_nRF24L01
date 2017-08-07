#ifndef _NRF24L01_H
#define _NRF24L01_H
#include <avr/io.h>
#include <avr/interrupt.h>
#define  uchar unsigned char
#define uint unsigned int
//spi symbol
#define DDR_SPI DDRB
#define DD_MOSI DDB3
#define DD_MISO DDB4
#define DD_SCK DDB5
#define DD_SS DDB2
#define DD_CE DDB1
#define DD_IRQ DDD2
#define CE PINB1
#define IRQ PIND2
#define MISO PINB4
#define   High_24L01_MISO	 PORTB |= (1 << PINB4)
#define   Low_24L01_MISO     PORTB &= ~(1 << PINB4)
#define   Read_24L01_MISO    PINB & (1 << PINB4)
#define   High_24L01_MOSI    PORTB |= (1 << PINB3)
#define   Low_24L01_MOSI     PORTB &= ~(1 << PINB3)
#define   Read_24L01_MOSI    PINB & (1 << PINB3)
#define   High_24L01_SCK	 PORTB |= (1 << PINB5)
#define   Low_24L01_SCK      PORTB &= ~(1 << PINB5)
#define   Read_24L01_SCK     PINB & (1 << PINB5)
#define   Low_24L01_CSN      PORTB &= ~(1 << PINB2)
#define   High_24L01_CSN     PORTB |= (1 << PINB2)
#define   High_24L01_CE      PORTB |= (1 << PINB1)
#define   Low_24L01_CE       PORTB &= ~(1 << PINB1)
#define   Read_24L01_CE      PINB & (1 << PINB1)
//*******************************new
#define  High_24L01_IRQ		 PORTD |= (1 << PIND2)
#define  Low_24L01_IRQ		 PORTD &= ~(1 << PIND2)
//*********************************************NRF24L01
#define TX_ADR_WIDTH 5
#define RX_ADR_WIDTH 5
#define TX_PLOAD_WIDTH 32
#define RX_PLOAD_WIDTH 1

//*************************************REGISTER INSTRUCTION
#define READ_REG        0x00
#define WRITE_REG       0x20
#define RD_RX_PLOAD     0x61
#define WR_TX_PLOAD     0xA0
#define FLUSH_TX        0xE1
#define FLUSH_RX        0xE2
#define REUSE_TX_PL     0xE3
#define NOP1            0xFF
//*******************************REGISTER ADDRESS
#define CONFIG          0x00
#define EN_AA           0x01
#define EN_RXADDR       0x02
#define SETUP_AW        0x03
#define SETUP_RETR      0x04
#define RF_CH           0x05
#define RF_SETUP        0x06
#define STATUS          0x07
#define OBSERVE_TX      0x08
#define RPD             0x09
#define RX_ADDR_P0      0x0A
#define RX_ADDR_P1      0x0B
#define RX_ADDR_P2      0x0C
#define RX_ADDR_P3      0x0D
#define RX_ADDR_P4      0x0E
#define RX_ADDR_P5      0x0F
#define TX_ADDR         0x10
#define RX_PW_P0        0x11
#define RX_PW_P1        0x12
#define RX_PW_P2        0x13
#define RX_PW_P3        0x14
#define RX_PW_P4        0x15
#define RX_PW_P5        0x16
#define FIFO_STATUS     0x17
//***************************************************function
void io_init(void);
void delay_nus(uint n);
void delay_nms(uint n);
uchar SPI_Read(uchar reg);
uchar SPI_RW_Reg(uchar reg, uchar value);
uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar bites);
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bites);
void SetRX_Mode(void);
uchar nRF24L01_RxPacket(uchar* rx_buf);
void nRF24L01_TxPacket(uchar* tx_buf);
uchar CheckACK(void);
void init_NRF24L01(void);
//new functions to output to hyperterminal
uchar print_register_hyperterminal(uchar reg);
void print_rw_address(uchar reg);
uchar print_register_state(uchar state);
void print_register_hex(uchar state);
void SPI_FLUSH_TX(void);
void SPI_FLUSH_RX(void);


#endif


