/*
 * _3x4_key_pad.c
 * this code interface a key pad with timer0 as a stop watch 
 * Created: 9/6/2020 10:55:40 PM
 *  Author: mostafa gomaa
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*including key_pad header file*/
#include "key_pad.h"

#define no_of_overflows 1810/*number of overflows needed for counting one minute*/
u_int16 ticks ; /*global variable acts like counter of overflows*/

/*function happens when timer0 overflows*/
ISR(TIMER0_OVF_vect)
{
	
	
	ticks++;
	if(ticks == no_of_overflows)
	{
	if(PORTC > 0)
	{
		PORTC--;   /*decrement by one every minute*/
	    ticks =0; /*clear ticks to recounts*/
	}	
	}	
}
/*
 *function to configure timer0
 * configuration of timer0
 * prescaller -> 1024
 * mode -> normal mode
 * initial value -> zero
 */
void TMR0_initit(void)
{
	
	TCNT0 = 0;                                     /*initial value*/
	TIMSK = (1<<TOIE0);                            /*enabling interrupt for timer0*/
	TCCR0 |= ((1<<FOC0) | (1<<CS02) | (1<<CS00)); /*normal mode with 1042 prescaller*/
	
}

int main(void)
{
	
	          
	u_int8 num ;               /*variable to hold the pressed number*/
	sei();                    /*enable global interrupts*/
	DDRC = 0x0f;             /*first four bits of port c as output pins for the 7-segment*/
	//PORTC = 0;              /*the initial value of 7-segment is zero*/
	DDRB |= (1<<PB0);      /*alarm*/
	PORTB &= (~(1<<PB0)); /*alarm is off*/
	DDRB |= (1<<PB1);    /*red led*/
    PORTB &= (~(1<<PB1));/*led is off*/
	
	 
    while(1)
    {
		
        num = pressed_key1();
			if((num >= 0) && (num <= 9))
		{
			PORTC = num; /*display the pressed num*/
		}
		TMR0_initit();
		while(PORTC != 0);
		{
				 
		PORTB |= (1<<PB0);  /*buzzer*/ 
		PORTB |= (1<<PB1); /*red led*/
		_delay_ms(5000);
		PORTB &= (~(1<<PB0)); 
		PORTB &= (~(1<<PB1));
		_delay_ms(5000);
		}/*end of while*/
	}/*end of while(1)*/
}/*end of main*/