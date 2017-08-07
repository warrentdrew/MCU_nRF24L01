//**************************************************************
// ****** FUNCTIONS FOR SPI COMMUNICATION *******
//**************************************************************
//Controller: ATmega328P (Clock: 3.6864 Mhz-internal)
//**************************************************************

#include <avr/io.h>
#include "SPI_routines.h"

//SPI initialize 

void spi_init(void)
{
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);//enable SPI, master, fosc/64
	SPSR = 0x00;
}

uchar SPI_RW(uchar data)
{
	// Start transmission
	SPDR = data;

	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}


//******** END ****** www.dharmanitech.com *****
