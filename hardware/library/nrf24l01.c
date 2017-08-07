#include "nrf24l01.h"
#include "usart_routines.h"
#include "SPI_routines.h"


void io_init(void){					//i/o initialization
	High_24L01_CSN;					//set CSN to high(not active)
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS)|(1 <<DD_CE);//set MOSI,SCK,CSN,CE as output
	DDR_SPI &= ~(1<<DD_MISO);		//MISO input
	DDRD &= ~(1<<DD_IRQ);			//IRQ input
	DDRB |= (1<<DDB0);				//LED signal

}

void delay_nus(uint n) {
	while(n--)
	;
}
void delay_nms(uint n) {
	uint j;
	while(n--)
	for(j=0;j<1140;j++);
}

//***************************************flush tx
void SPI_FLUSH_TX(void){
	Low_24L01_CSN;
	delay_nus(10);
	SPI_RW(FLUSH_TX);
	delay_nus(10);
	High_24L01_CSN;

}
//************************************flush rx
void SPI_FLUSH_RX(void){
	Low_24L01_CSN;
	delay_nus(10);
	SPI_RW(FLUSH_RX);
	delay_nus(10);
	High_24L01_CSN;

}


//*********************************** read nRF24L01 register
uchar SPI_Read(uchar reg) {
	uchar reg_val;
	Low_24L01_CSN;
	delay_nus(10);
	SPI_RW(reg);
	delay_nus(10);
	reg_val = SPI_RW(0xff);
	delay_nus(10);
	High_24L01_CSN;
	return(reg_val);
}
//************************************** write nRF24L01 register
uchar SPI_RW_Reg(uchar reg, uchar value) {
	uchar status;
	Low_24L01_CSN;
	delay_nus(10);
	status = SPI_RW(reg);
	delay_nus(10);
	SPI_RW(value);
	delay_nus(10);
	High_24L01_CSN;
	return(status);
}
//******************************************read RX-fifo buffer or register with 5 bytes
uchar SPI_Read_Buf(uchar reg, uchar *pBuf, uchar bites) {
	uint status1,uchar_ctr;
	Low_24L01_CSN;
	delay_nus(10);
	status1 = SPI_RW(reg);
	delay_nus(10);
	for(uchar_ctr=0;uchar_ctr<bites;uchar_ctr++){
	pBuf[uchar_ctr] = SPI_RW(0xff);
	delay_nus(10);
	}
	High_24L01_CSN;
	return(status1);
}
//**************************************write TX-FIFO register
uchar SPI_Write_Buf(uchar reg, uchar *pBuf, uchar bites) {
	uchar status1,uchar_ctr;
	Low_24L01_CSN;				//SPI enable
	delay_nus(10);
	status1 = SPI_RW(reg);
	delay_nus(10);
	for(uchar_ctr=0; uchar_ctr<bites; uchar_ctr++){ //write more than 1 byte into bufferL
	SPI_RW(*pBuf++);
	delay_nus(10);
	}
	High_24L01_CSN;			//SPI disable
	return(status1);			
}

void init_NRF24L01(void) {
	uart0_init();		//I/O initialization
	io_init();
	spi_init();
	Low_24L01_CE;			//standby mode
	High_24L01_CSN;		//disable SPI
	Low_24L01_SCK;		//disable clock
	High_24L01_IRQ;		//no interruption
	
}










