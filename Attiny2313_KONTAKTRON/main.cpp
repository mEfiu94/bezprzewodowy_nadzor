/*

 * main.cpp

 *
 *  Created on: 05.10.2016
 *      Author: mEfiu
 */
#define GIFR _SFR_IO8(0x3A)
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD_HD44780.h"
#include "nRF.hpp"
#include "GLOBAL.h"
volatile uint8_t i=0;
volatile uint8_t flag=255;
volatile uint8_t stan=0;
char* byte_to_text(uint8_t value){
	char txt[11];
	itoa(value,txt,2);
	return txt;
}
uint8_t get_bit(uint8_t reg,uint8_t bit){
	return ((nRF::read_register_byte(reg)>>bit)&0x01);
}

int main(){
	MCUCR|=(1<<ISC00);
	GIMSK|=(1<<INT1)|(1<<INT0);
	GIFR|=(1<<INTF1)|(1<<INTF0);
	ACSR|=(1<<ACD);//ADC off
	DDRD|=(1<<PD5);
	PORTD|=(1<<PD3)|(1<<PD2);
	sei();
	nRF::init();
	_delay_ms(50);
	nRF::config(0);
	_delay_ms(5);
	nRF::TX_send(0x1f);
	_delay_ms(10);
	set_sleep_mode(SLEEP_MODE_IDLE);
	while(1){
		sleep_mode();
		i++;
		_delay_ms(100);
	}
}

ISR(INT1_vect){//PIN IRQ NADAJNIK
	uint8_t status=nRF::read_register_byte(STATUS);
	if((status>>TX_DS)&0x01){
		flag=1;
		nRF::write_register_byte(STATUS,status|(1<<TX_DS));
	}else if((status>>MAX_RT)&0x01){
		flag=0;
		nRF::write_register_byte(STATUS,(1<<MAX_RT));
	}else
		flag=100;
}
ISR(INT0_vect){
	if(stan==0){
		nRF::TX_send(0x11);
		stan=1;
	}else{
		nRF::TX_send(0x10);
		stan=0;
	}
}
