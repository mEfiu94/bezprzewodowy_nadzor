/*
 * spi.cpp
 *
 *  Created on: 06.10.2016
 *      Author: mEfiu
 */

#include "usi_spi.hpp"


void USI_SPI::spi_master_init(){
	sbi(SPI_DDRX,SPI_DO);
	sbi(SPI_DDRX,SPI_CLK);

}
void USI_SPI::spi_transmit_bytes(uint8_t *bytes,uint8_t length){
	for(uint8_t i=0;i<length;i++){
		USIDR=bytes[i];
		USISR=_BV(USIOIF); //Wyzeruj licznik
			 do
			  {
			   USICR=_BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
			  } while((USISR & _BV(USIOIF))==0);

	}
}
uint8_t USI_SPI::spi_transmit(uint8_t data){
	USIDR=data;
	 USISR=_BV(USIOIF); //Wyzeruj licznik
	 do
	  {
	   USICR=_BV(USIWM0) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
	  } while((USISR & _BV(USIOIF))==0);
	 return USIDR;
}
void USI_SPI::spi_slave_init(){
	sbi(SPI_DDRX,SPI_DO);
	cbi(SPI_DDRX,SPI_DI);
	cbi(SPI_DDRX,SPI_CLK);
	cbi(SPI_DDRX,SPI_SS);
	USICR=(1<<USIOIE)|(1<<USIWM0)|(1<<USICS0)|(1<<USICS1);
	USISR=(1<<USIOIF);
	sei();
}
