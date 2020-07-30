#include "LCD_HD44780.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include "nRF.hpp"
uint8_t get_bit(uint8_t reg,uint8_t bit){
	return ((nRF::read_register_byte(reg)>>bit)&0x01);
}
volatile uint8_t bufor=0x00,data_pipe=0xff;
char* byte_to_text(uint8_t value){
	char txt[11];
	itoa(value,txt,2);
	return txt;
}
ISR(INT1_vect){//obsluga przerwania pin IRQ, ODBIORNIK
	if( get_bit(STATUS,RX_DR)){
		data_pipe=get_bit(STATUS,RX_P_NO);
		bufor=nRF::RX_command();
		nRF::write_register_byte(STATUS,(1<<RX_DR));
	}
}

int main() {
	uint8_t dataa[5]={0x00,0x00,0x00,0x00,0x00};
	GICR|=(1<<INT1);
	GIFR|=(1<<INTF1);
	PORTD|=1<<PD3;
	sei();
	LCD_HD44780::init();
	LCD_HD44780::clear();
	char* wiadomosc="";
	char* wiadomosc2="";
	nRF::init();
	nRF::config(1);
	_delay_ms(5);
	LCD_HD44780::clear();
	LCD_HD44780::writeText("System nadzoru");
	_delay_ms(2000);

	while (1) {
		LCD_HD44780::clear();

		if (bufor == 0x20) {
			wiadomosc2 = "RUCH";
			bufor = 0;
		}
		if (bufor == 0x21) {
			wiadomosc2 = "BRAK RUCHU";
			bufor = 0;
		}

		if (bufor == 0x11) {
			wiadomosc = "OTWARTE";
			bufor = 0;
		}
		if (bufor == 0x10) {
			wiadomosc = "ZAMKNIETE";
			bufor = 0;
		}
		if (bufor == 0x1f) {
					wiadomosc = "Kontaktron ON";
					bufor = 0x10;
		}
		if (bufor == 0x2f) {
					wiadomosc2 = "PIR ON - wait";
					bufor = 0;
		}
		LCD_HD44780::writeText(wiadomosc2);
		LCD_HD44780::goTo(0, 1);
		LCD_HD44780::writeText(wiadomosc);
		_delay_ms(200);
	}

}

