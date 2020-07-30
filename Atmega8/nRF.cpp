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
	spi_fast_shift(FLUSH_TX);
	CSN_SET_HIGH;
}
void nRF::TX_send(uint8_t packet){
	TX_ON;
	nRF::flush_tx();

	CSN_SET_LOW;
	spi_fast_shift(W_TX_PAYLOAD);
	spi_fast_shift(packet);
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
	spi_fast_shift(R_RX_PAYLOAD);
	val=spi_fast_shift(NOP);
	CSN_SET_HIGH;
	return val;
}
uint8_t nRF::read_register_byte(uint8_t reg_address){

	CSN_SET_LOW;
	spi_fast_shift(R_REGISTER|(REGISTER_MASK&reg_address));
	uint8_t value=spi_fast_shift(NOP);
	CSN_SET_HIGH;
	return value;
}
void nRF::read_register_fivebytes(uint8_t reg_address,uint8_t data_out[5]){
	CSN_SET_LOW;
	spi_fast_shift(R_REGISTER|(REGISTER_MASK&reg_address));
	for(uint8_t i=0;i<5;i++)
		data_out[i]=spi_fast_shift(NOP);
	CSN_SET_HIGH;

}
void nRF::write_register_byte(uint8_t reg_address,uint8_t byte){
	CSN_SET_HIGH;
	CSN_SET_LOW;
	spi_fast_shift(W_REGISTER|(REGISTER_MASK&reg_address));
	spi_fast_shift(byte);
	CSN_SET_HIGH;
}
void nRF::write_register_five_bytes(uint8_t reg_address,uint8_t* five_bytes){
	CSN_SET_LOW;
	spi_fast_shift(W_REGISTER|(REGISTER_MASK&reg_address));
	spi_transmit_sync(five_bytes,5);
	CSN_SET_HIGH;
}
void nRF::config(uint8_t receiver){
	uint8_t addr_P1[]={RX_ADDRP0,RX_ADDRP0,RX_ADDRP0,RX_ADDRP0,RX_ADDRP0};
	write_register_byte(RF_CH,RF_kanal);

	if(receiver){
		RX_ON;
		write_register_byte(RX_PW_P0,1);
		write_register_byte(RX_PW_P1,1);
		nRF::write_register_byte(EN_RXADDR,(1<<ERX_P0)|(1<<ERX_P1));
		nRF::write_register_byte(EN_AA,(1<<ENAA_P0)|(1<<ENAA_P1));
		PRX=1;

		CE_SET_HIGH;
	}else{
		PRX=0;
		//nRF::write_register_five_bytes(TX_ADDR,0xE7E7E7E7E7ULL);
		//nRF::write_register_five_bytes(RX_ADDR_P0,0xE7E7E7E7E7ULL);
		write_register_byte(EN_RXADDR,(1<<ERX_P0)|(1<<ERX_P1));
		TX_ON;
	}
}
void nRF::init(){
	spi_init();
	DDRB|=(1<<CE)|(1<<CSN); //CE i CSN jako wyjscia
	CE_SET_LOW;//odbieranie danych - 1, nadawanie -0
	CSN_SET_HIGH;//wysylanie komend lub czytanie danych
}

