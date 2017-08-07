//**************************************************************
// ****** FUNCTIONS FOR SPI COMMUNICATION *******
//**************************************************************
//Controller: ATmega328P (Clock: 3.6864 Mhz-internal)
//**************************************************************
#include "nrf24l01.h"

#ifndef _SPI_ROUTINES_H_
#define _SPI_ROUTINES_H_

void spi_init(void);
uchar SPI_RW(uchar data);


#endif
