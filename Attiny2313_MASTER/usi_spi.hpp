/*
 * spi.hpp
 *
 *  Created on: 06.10.2016
 *      Author: mEfiu
 */

#ifndef USI_SPI_HPP_
#define USI_SPI_HPP_
#include "GLOBAL.h"

#define SPI_DDRX DDRB
#define SPI_CLK PB7
#define SPI_DI PB5
#define SPI_DO PB6
#define SPI_SS PB4


class USI_SPI{
public:
	void static spi_master_init();
	uint8_t static spi_transmit(uint8_t data);
	void static spi_slave_init();

};




#endif /* USI_SPI_HPP_ */
