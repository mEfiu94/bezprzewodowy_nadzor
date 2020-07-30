/*
 * nRF.hpp
 *
 *  Created on: 12.10.2016
 *      Author: mEfiu
 */

#ifndef NRF_HPP_
#define NRF_HPP_
#include "avr/io.h"
#include "LCD_HD44780.h"
#include "spi.hpp"
#define nrf_cfg (1<<EN_CRC)|(1<<CRC0) //
#define RF_kanal 2
#define Payload 1
#define RX_ADDRP0 0xE7
#define TX_ON nRF::write_register_byte(CONFIG,(1<<EN_CRC)|(1<<CRC0)|(1<<PWR_UP)|(0<<PRIM_RX))
#define RX_ON nRF::write_register_byte(CONFIG,(1<<EN_CRC)|(1<<CRC0)|(1<<PWR_UP)|(1<<PRIM_RX))
//Registers' Addresses
#define CONFIG 0x00 //Configuration Register
#define EN_AA 0x01 //Enable Auto Ack Register
#define EN_RXADDR 0x02 //Enable RX Addresses Register
#define SETUP_AW 0x03 //Setup of Address Widths
#define SETUP_RETR 0x04 //Setup of Automatic Retransmission
#define RF_CH 0x05//RF Channel
#define RF_SETUP 0x06 //RF Setup Register
#define STATUS 0x07 //Status Register
#define OBSERVE_TX 0x08 //Transmit observe register
#define RPD 0x09
#define RX_ADDR_P0 0x0A //Receive address data pipe0
#define RX_ADDR_P1 0x0B //Receive address data pipe1
#define RX_ADDR_P2 0x0C //Receive address data pipe2
#define RX_ADDR_P3 0x0D //Receive address data pipe3
#define RX_ADDR_P4 0x0E //Receive address data pipe4
#define RX_ADDR_P5 0x0F //Receive address data pipe5
#define TX_ADDR 0x10
#define RX_PW_P0 0x10
#define RX_PW_P1 0x11
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17// FIFO Status Register
#define DYNPD 0x1C //Dynamic payload length enable register
#define FEATURE 0x1D //Feature Register
//MNEMONICS
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRC0        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define PLL_LOCK    4
#define RF_DR       3
#define RF_PWR      1
#define LNA_HCURR   0
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0
/* Instruction Mnemonics */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define NOP           0xFF
//PINY
#define CE PB0
#define CSN PB1
#define CE_SET_HIGH PORTB|=(1<<CE)
#define CE_SET_LOW PORTB&=~(1<<CE)
#define CSN_SET_HIGH PORTB|=(1<<CSN)
#define CSN_SET_LOW PORTB&=~(1<<CSN)

class nRF{
public:
	static void config(uint8_t receiver);
	static void init();
	static uint8_t  read_register_byte(uint8_t reg_address);
	static void write_register_byte(uint8_t reg_address,uint8_t bit);
	static void write_register_five_bytes(uint8_t reg_address,uint8_t* five_bytes);
	static uint8_t RX_command();
	static void TX_send(uint8_t packet);
	static void flush_tx();
	static uint8_t max_rt_reached();
	void read_register_fivebytes(uint8_t reg_address,uint8_t data_out[5]);
};



#endif /* NRF_HPP_ */
