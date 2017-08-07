#define F_CPU 3686400UL      //To make the delay function work correctly

#include <util/delay.h>
#include "nrf24l01.h"
#include "usart_routines.h"
#include "SPI_routines.h"
//address for transmitting and receiving
uchar TX_ADDRESS[RX_ADR_WIDTH]={0x43,0x11,0x22,0x27,0x37};
uchar RX_ADDRESS[RX_ADR_WIDTH]={0x43,0x11,0x22,0x27,0x37};

int main(void){

	uchar TX_BUF[32];			//buffer for transmitting data
	uchar sta1,tx_data,rx_data;
	uchar n=0;					
	int flag=0;

	init_NRF24L01();
	delay_nms(100);

	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);				//enable auto-acknowledge on pipe 0
	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x2f);			//wait 750+86us in the interval 
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);			//enable rx address on pipe 0
	SPI_RW_Reg(WRITE_REG + SETUP_AW, 0x03);				//address width 5 bytes
	SPI_RW_Reg(WRITE_REG + RF_CH, 0x40);				//channel 2.4GHz+40MHz
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x00);				//1Mbps data rate, -18dBm
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, TX_ADR_WIDTH);		//set rx address for pipe0 for receiving acknowledge
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);			//set tx address
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);		//set payload width of the rx pipe0	
	delay_nms(500);

	while(1){
		flag=0;
			SPI_FLUSH_TX();								//flush RX-FIFO and TX-FIFO
			SPI_FLUSH_RX();
	
		//library is first set to receiver then transmitter
		
			Low_24L01_CE;
			SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);		//set to receiver
			High_24L01_CE;								//start monitor
			delay_nus(200);								//delay
			
			while(!flag){
				sta1=SPI_Read(STATUS);
				if(sta1&0x40){
					Low_24L01_CE;				//stop monitor
					SPI_Read_Buf(RD_RX_PLOAD,&rx_data,RX_PLOAD_WIDTH);		//read data from RX_FIFO buffer
					SPI_RW_Reg(WRITE_REG+STATUS,sta1);			//set RX_DR,TX_DS,MAX_PT to 1£¬to erase interruption flag
					flag=1;						//end flag of receiving data
				}
				else{
					flag=0;
				}
				
			}
			flag=0;
			transmitByte('#');					//send start symbol for command
			delay_nms(100);
			transmitByte(rx_data);				//send command(file number)
			delay_nms(100);
			tx_data=receiveByte();				//start receive file content
			n=0;
			if(tx_data=='*'){
				do{								//put all the file content into the TX_BUF
					TX_BUF[n]=tx_data;
					tx_data=receiveByte();
					n++;
				}while (tx_data!='$');	
			TX_BUF[n]=tx_data;					//put end mark "$" into TX_BUF
			delay_nms(1500);						//delay should be long enough to wait for operator side for receiving
			SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);	//set to transmitter
			PORTB |= (1<<PINB0);					//LED signal
			
			while(!flag){
				SPI_FLUSH_TX();
				Low_24L01_CE;
				SPI_Write_Buf(WR_TX_PLOAD, TX_BUF, TX_PLOAD_WIDTH);	//tx_payload operation to send data
				
				delay_nms(500);						//delay should be long enough
				High_24L01_CE;						//start sending
				delay_nus(50);
				sta1=SPI_Read(STATUS);				//read out STATUS register
				delay_nms(800);
				if(sta1&0x20)						//if tx_ds
					{
						SPI_RW_Reg(WRITE_REG+STATUS,0xff);		//flush TX_DS and MAX_RT
						SPI_FLUSH_TX();					//flush TX-FIFO
						flag=1;
					}
				else{
					flag=0;	
				}
			}
		}
	}	
	return 0;
}
	
	