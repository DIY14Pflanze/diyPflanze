/*
 * Wasserwerk_mega8.c
 *
 * Created: 20.02.2015 12:55:02
 *  Author: Mattan
 */ 



#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

volatile uint_fast64_t counter =0x400;
volatile int_least8_t aa =0;

void init(void)
{
	DDRC |= (1 << PC2)|(1<<PC1);
	
	 
	 DDRD &= ~(1 << PD2); // INT0: input...
	 PORTD |= (1 << PD2); // ...with pullup.
	 
	 // level interrupt INT0 (low level)
	 MCUCR &= ~((1 << ISC01) | (1 << ISC00));
	 
	
	
	// Timer 0 konfigurieren
	
	TCCR2 |= (1<<CS02);
	TCCR2 |= (1<<CS00); // Prescaler 1024

	
	// Overflow Interrupt erlauben
	TIMSK |= (1<<TOIE2);
	

set_sleep_mode (SLEEP_MODE_ADC);
	
}

ISR(TIMER2_OVF_vect)
{
	
}

 ISR(INT0_vect)
 {
	 aa=1;
 }

int main(void)
{
	init();
	sleep_enable();
	PORTC |= (1 << PC1);
	while(counter>= 1){
		cli();
		counter--;
		sei();
		sleep_cpu();
	}
	
	while(1){
		
		PORTC &= ~(1<<PC1);
		cli();
		counter=0x00949000;
		
		sei();
		while(counter>=0x200){
			sleep_cpu();
	
			cli();
			counter--;
			sei();
		}
		
		PORTC |= (1 << PC1);
		
		while(counter>=1){
			sleep_cpu();
			cli();
			counter--;
			sei();
		}
			
		
			
		
	}
}
