/*
 * nRf.cpp
 *
 *  Created on: 17.10.2016
 *      Author: mEfiu
 */

#include "nRF.hpp"

volatile uint8_t PRX;

void nRF::flush_tx(){
	CSN_SET_LOW;
	USI_SPI::spi_transmit(FLUSH_TX);
	CSN_SET_HIGH;
}
void nRF::TX_send(uint8_t packet){
	TX_ON;
	nRF::flush_tx();

	CSN_SET_LOW;
	USI_SPI::spi_transmit(W_TX_PAYLOAD);
	USI_SPI::spi_transmit(packet);
	CSN_SET_HIGH;

	CE_SET_HIGH;
	_delay_us(20);
	CE_SET_LOW;
}
uint8_t nRF::max_rt_reached(){
	return nRF::read_register_byte(STATUS)&(1<<MAX_RT);
}
uint8_t nRF::RX_command(){
	uint8_t val;
	CSN_SET_LOW;
	USI_SPI::spi_transmit(R_RX_PAYLOAD);
	val=USI_SPI::spi_transmit(0x00);
	CSN_SET_HIGH;
	return val;
}
uint8_t nRF::read_register_byte(uint8_t reg_address){

	CSN_SET_LOW;
	USI_SPI::spi_transmit(R_REGISTER|(REGISTER_MASK&reg_address));
	uint8_t value=USI_SPI::spi_transmit(NOP);
	CSN_SET_HIGH;
	return value;
}
void nRF::write_register_byte(uint8_t reg_address,uint8_t byte){
	CSN_SET_HIGH;
	CSN_SET_LOW;
	USI_SPI::spi_transmit(W_REGISTER|(REGISTER_MASK&reg_address));
	USI_SPI::spi_transmit(byte);
	CSN_SET_HIGH;
}
void nRF::write_register_five_bytes(uint8_t reg_address,uint8_t *five_bytes){
	CSN_SET_LOW;
	USI_SPI::spi_transmit(W_REGISTER|(REGISTER_MASK&reg_address));
	USI_SPI::spi_transmit_bytes(five_bytes,5);
	CSN_SET_HIGH;
}
void nRF::config(uint8_t receiver){
	uint8_t addr[]={TX_addr,TX_addr,TX_addr,TX_addr,TX_addr};
	write_register_byte(RF_CH,RF_kanal);
	if(receiver){
		PRX=1;
		write_register_byte(RX_PW_P0,1);
		write_register_byte(RX_PW_P1,1);
		write_register_byte(CONFIG,(nrf_cfg|(1<<PWR_UP)|(1<<PRIM_RX))); //cfg stale, wlaczenie nrf i odbieranie
		CE_SET_HIGH;
	}else{
		PRX=0;
		TX_ON;
		nRF::write_register_five_bytes(TX_ADDR,addr);
		nRF::write_register_five_bytes(RX_ADDR_P0,addr);

	}
}
void nRF::init(){
	USI_SPI::spi_master_init();
	DDRB|=(1<<CE)|(1<<CSN); //CE i CSN jako wyjscia
	CE_SET_LOW;//odbieranie danych - 1, nadawanie -0
	CSN_SET_HIGH;//wysylanie komend lub czytanie danych
}

