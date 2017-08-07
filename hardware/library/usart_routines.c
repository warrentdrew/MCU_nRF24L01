#include "usart_routines.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>


void uart0_init(void)
{
	UCSR0B = 0x00;		//disable while setting baud rate
	UCSR0A = 0x00;		//clear status flags
	UCSR0C = 0x06;		//choosing character size of 8-bit
	UBRR0L = 0x0B;		//set baud rate lo
	UBRR0H = 0x00;		//set baud rate hi
	UCSR0B = 0x18;		//Receiver Enable, Transmitter Enable
}



void transmitString_F(char* string)
{
	while (pgm_read_byte(&(*string))){
	transmitByte(pgm_read_byte(&(*string++)));
	
	}
}



void transmitByte( unsigned char data )
{
	while ( !(UCSR0A & (1<<UDRE0)) )
	; 			                /* Wait for empty transmit buffer */
	UDR0 = data; 			        /* Start transmission */
}


unsigned char receiveByte( void )
{
	unsigned char data, status;
	
	while(!(UCSR0A & (1<<RXC0))); 	// Wait for incoming data
	
	status = UCSR0A;
	data = UDR0;
	
	return(data);
}
