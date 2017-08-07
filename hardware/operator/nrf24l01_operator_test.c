#define F_CPU 3686400UL      //To make the delay function work correctly

#include <util/delay.h>
#include "nrf24l01.h"
#include "usart_routines.h"

//address for transmitting and receiving
uchar TX_ADDRESS[RX_ADR_WIDTH]={0x43,0x11,0x22,0x27,0x37};
uchar RX_ADDRESS[RX_ADR_WIDTH]={0x43,0x11,0x22,0x27,0x37};

int main(void){
	uchar RX_BUF[32];
	uchar sta1,data;
	int flag=0;

	init_NRF24L01();			//all the initialization process
	delay_nms(300);

	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);					//enable auto-ack on pipe 0
	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x2f);				//750+86us
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);				//enable re address on pipe 0	
	SPI_RW_Reg(WRITE_REG + SETUP_AW, 0x03);					//5 bytes address width
	SPI_RW_Reg(WRITE_REG + RF_CH, 0x40);					//channel 2.4GHz+40MHz
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x00);					//1 Mbps, -18dBm
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, TX_ADR_WIDTH);		//set rx address for pipe0 for receiving acknowledge
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);			//set tx address
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);		//set payload width of the rx pipe0	
	delay_nms(500);

	while(1){
		flag=0;
		Low_24L01_CE;
		SPI_FLUSH_TX();					//flush RX-FIFO and TX-FIFO
		delay_nms(100);
		SPI_FLUSH_RX();
		delay_nms(100);
		SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);			//set the module into a transmitter
		delay_nms(200);
		
		data=receiveByte();				//start receiving command from user input
		if(data=='#'){							
			PORTB |= (1<<PINB0);
			data=receiveByte();			//receive file number
			delay_nms(200);
			
			while(!flag){
			transmitByte(data);			
			delay_nms(100);
			Low_24L01_CE;
			SPI_Write_Buf(WR_TX_PLOAD, &data, TX_PLOAD_WIDTH);	//tx_payload operation to send data
			
			High_24L01_CE;
			delay_nus(50);

			sta1=SPI_Read(STATUS);
			delay_nms(800);
			if(sta1&0x20)					//if tx_ds set
				{
					SPI_RW_Reg(WRITE_REG+STATUS,0xff);		//flush TX_DS and MAX_RT
					SPI_FLUSH_TX();			//flush TX-FIFO
					flag=1;
				}
			else{
				flag=0;	
			}
		}
		flag=0;
		//**********************************************************************become a receiver 
			Low_24L01_CE;
			SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);		//set to receiver
			High_24L01_CE;								//start monitor
			delay_nus(200);								//delay
			delay_nms(500);
			while(!flag){
				sta1=SPI_Read(STATUS);					//read out STATUS register
				if(sta1&0x40){
					Low_24L01_CE;						//stop monitoring
					SPI_Read_Buf(RD_RX_PLOAD,RX_BUF,RX_PLOAD_WIDTH);		//read data from RX_FIFO buffer
					SPI_RW_Reg(WRITE_REG+STATUS,sta1);			//set RX_DR,TX_DS,MAX_PT to 1£¬to erase interruption flag
					flag=1;					//end flag of receiving data
				}
				else{
					flag=0;		
				}
				
			}
			for(int i=0;i<RX_PLOAD_WIDTH;i++){		//send back the data received to the terminal 
				PORTB |=(1<<PINB0);					//LED flash signals on
				transmitByte(RX_BUF[i]);
				delay_nms(100);
				PORTB &=~(1<<PINB0);				//LED flash signals off
				delay_nms(100);
			}
				
		}

	}
	return 0;
}
	
	