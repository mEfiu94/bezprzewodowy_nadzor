#define GIFR _SFR_IO8(0x3A)
#include "LCD_HD44780.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "nRF.hpp"
uint8_t get_bit(uint8_t reg,uint8_t bit){
	return ((nRF::read_register_byte(reg)>>bit)&0x01);
}
volatile uint8_t bufor=0,data_pipe;

ISR(INT1_vect){//obsluga przerwania dotyczacego nRF
	CE_SET_LOW;
	if( ((nRF::read_register_byte(STATUS)>>RX_DR)&0x01)){
		data_pipe=get_bit(STATUS,RX_P_NO);
		bufor=nRF::RX_command();
		uint8_t reg_temp=nRF::read_register_byte(STATUS); //czyszczenie przerwania RX
		nRF::write_register_byte(STATUS,reg_temp|(1<<RX_DR));
	}
	PORTB|=1<<PB1;
	CE_SET_HIGH;
}
char* byte_to_text(uint8_t value){
	char txt[11];
	itoa(value,txt,2);
	return txt;
}
int main() {
	GIMSK|=1<<INT1;
	GIFR|=1<<INTF1;
	PORTD|=1<<PD3;
	sei();
	DDRA|=1<<PA0;
	nRF::init();
	nRF::config(1);
	LCD_HD44780::init();
	LCD_HD44780::clear();
	_delay_ms(20);
	while (1) {
		LCD_HD44780::clear();
		//uint8_t val=nRF::read_register_byte(CONFIG);
		LCD_HD44780::writeText("test");
		_delay_ms(100);
	}

}

