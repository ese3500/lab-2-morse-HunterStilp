/*
 * ESE 350 Lab 2.c
 *
 * Created: 2/10/2023 11:28:05 AM
 * Author : hunter
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"

char String[25];

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

int count = 0;
char button_presses[40];

typedef struct {
	char *key;
	char *value;
} Dictionary;

Dictionary mapping[26];

void initialize()
{
	
	//Serial
	cli();
	UART_init(BAUD_PRESCALER); 
	
	//Set up timer 0. will be used for counting to 10ms. Used for timing
	TCCR0A = 0b00000000;
	TCCR0B = 0b00001101;//sets prescaler to 1024/ ie 16mhz/1024 = 15.625khz
	
	OCR0A = 156-1;//want to increase clock every 10ms; 10ms/(1/15.625khz)=156
	
	TIMSK0 = 0b00000010;//Turns on Timer/Counter0 Output Compare A Match Interrupt
	
	
	
	//Set up for timer 1. Will be used for creating a ISR when it is time to disable the LED
	TCCR1A = 0b00000000;
	TCCR1B = 0b00001101;//sets prescaler to 1024/ ie 16mhz/1024 = 15.625khz
	
	OCR1A = 1563-1;//15625hz*0.1sec= 1563 cycles
	
	TIMSK1 = 0b00000010;// Turns on Timer/Counter1 Output Compare A Match Interrupt.
	
	
	
	//Set up interupt for pin 8
	PCMSK0 = 0b00000001;
	PCICR = 0b00000001;
	
	
	
	//Set up PB1/2 as output
	DDRB |= (1<<DDB1)|(1<<DDB2);
	
	define_mapping();
	
	//enable global interrupts
	sei();
}

void define_mapping()
{
	mapping[0].key = "*_";mapping[0].value = "A";
	mapping[1].key = "_***";mapping[1].value = "B";
	mapping[2].key = "_*_*";mapping[2].value = "C";
	mapping[3].key = "_**";mapping[3].value = "D";
	mapping[4].key = "*";mapping[4].value = "E";
	mapping[5].key = "**_*";mapping[5].value = "F";
	mapping[6].key = "__*";mapping[6].value = "G";
	mapping[7].key = "****";mapping[7].value = "H";
	mapping[8].key = "**";mapping[8].value = "I";
	mapping[9].key = "*____";mapping[9].value = "J";
	mapping[10].key = "_*_";mapping[10].value = "K";
	mapping[11].key = "*_**";mapping[11].value = "L";
	mapping[12].key = "__";mapping[12].value = "M";
	mapping[13].key = "_*";mapping[13].value = "N";
	mapping[14].key = "___";mapping[14].value = "O";
	mapping[15].key = "*__*";mapping[15].value = "P";
	mapping[16].key = "__*_";mapping[16].value = "Q";
	mapping[17].key = "*_*";mapping[17].value = "R";
	mapping[18].key = "***";mapping[18].value = "S";
	mapping[19].key = "_";mapping[19].value = "T";
	mapping[20].key = "**_";mapping[20].value = "U";
	mapping[21].key = "***_";mapping[21].value = "V";
	mapping[22].key = "*__";mapping[22].value = "W";
	mapping[23].key = "_**_";mapping[23].value = "X";
	mapping[24].key = "_*__";mapping[24].value = "Y";
	mapping[25].key = "__**";mapping[25].value = "Z";
}

char *get_char(char *key) {
	int i;

	// Search the dictionary for the given key
	for (i = 0; i < 26; i++) {
		if (strcmp(mapping[i].key, key) == 0) {
			// Return the value associated with the key
			return mapping[i].value;
		}
	}

	// If the key was not found, return null
	return '\0';
}




void print_con(char *str)
{
	sprintf(String, str);
	UART_putstring(String);
}






ISR(TIMER0_COMPA_vect){
	count ++;
}

ISR(TIMER1_COMPA_vect){
	PINB &= ~(1<<PINB1);
	PINB &= ~(1<<PINB2);
}

void turn_on_led(int num)
{
	if (num==1)
	{
		PINB |= (PINB1<<1);
		TCNT1 = 0;
		
	} else if (num==2)
	{
		PINB |= (PINB2<<1);
		TCNT1 = 0;
	}
}

//Will handle button press
ISR(PCINT0_vect){
	if((PINB & 1)==1)
	{//button was released
		if(count > 100)//changing to 1sec
		{
			turn_on_led(1);
			count = 0;
			strcat(button_presses, "_");
// 			print_con(get_char(button_presses));
// 			print_con("\n");
		}
		else if(count > 50)//changing to 0.5sec
		{
			turn_on_led(2);
			count = 0;
			strcat(button_presses, "*");
// 			print_con(get_char(button_presses));
// 			print_con("\n");
		}
	}
	else
	{//button was pressed
		if(count > 100)//increased time to 1s
		{
			//print_con("space \n");
			count = 0;
			print_con(get_char(button_presses));
			print_con("\n");
			memset(button_presses, 0, 40);
		}
	}
}


int main(void)
{
	
	initialize();

	
	while (1)
	{
	}
}
