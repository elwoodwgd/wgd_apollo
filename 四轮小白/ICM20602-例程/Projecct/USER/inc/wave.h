/*
 * wave.h
 *
 *  Created on: Nov 29, 2014
 *      Author: ass
 */
#ifndef WAVE_H_
#define WAVE_H_

#include "common.h"
#include "MK60_uart.h"


#define WAVE_UART_V2    uart4


void Uart1_Send_AF1(signed int aa,signed int bb,signed int cc,signed int dd,signed int ee,signed int ff,signed int gg,signed int hh);
unsigned char UART_Putc(unsigned char data);
void send_wave(void);

void printhh(void); 
void print5n(unsigned int x); 
void print4n(unsigned int x);
void print3n(unsigned int x);
void print2n(unsigned int x);

#endif /* WAVE_H_ */
