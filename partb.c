/*
 * ESE 350 Lab 2.c
 *
 * Created: 2/10/2023 11:28:05 AM
 * Author : hunter
this is first part of partB
 */ 

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

/*--------------------Libraries---------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
/*--------------------Variables---------------------------*/
char String[25];



#include <avr/io.h>
#include <avr/interrupt.h>


void initialize()
{
	cli();
	//Serial
/*	UART_init(BAUD_PRESCALER); */
	
	
	DDRB |= (1<<DDB4);//
	DDRB &= ~(1<<DDB0);
	PORTB |= (1<<PORTB0);
	

	
	//enables clock stuff
	TCCR1B &= ~(1<<ICES1);
	TIFR1 |=(1<<ICF1);
	TIMSK1 |=(1<<ICIE1);


	sei();
	
	
}


// void print_con(char *str)
// {
// 	sprintf(String, str);
// 	UART_putstring(String);
// }


int main(void)
{
	initialize();
	
	while (1)
	{
	//	_delay_ms(1000); 
		//print_con("Hello world! \n");
		
	}
	
}


ISR(TIMER1_CAPT_vect){
	if((PINB & 1)==1)
	{
		PORTB &=~(1<<PORTB4);
		TCCR1B &=~(1<<ICES1);
	} 
	else 
	{
		PORTB |=(1<<PORTB4);
		TCCR1B |=(1<<ICES1);
	}
}