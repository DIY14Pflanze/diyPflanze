/*
 * GießNano.c
 *
 * Created: 21.02.2015 13:28:23
 *  Author: Matthias
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define HELLIGKEIT
#define FEUCHTE 

volatile uint_fast64_t counter =0x400;
volatile int_least8_t aa =0;


uint16_t ADC_Read( uint8_t channel );
uint16_t ADC_Read_Avg( uint8_t channel, uint8_t nsamples );
void ADC_Init(void);


ISR(TIMER2_OVF_vect)
{
	
}

 ISR(INT0_vect)
 {
	 aa=1;
 }


void init(void)
{
	
	ADC_Init();
	DDRC |= (1 << PC2)|(1<<PC1);
	
	
	DDRD &= ~(1 << PD2); // INT0: input...
	PORTD |= (1 << PD2); // ...with pullup.
	
	// level interrupt INT0 (low level)
	MCUCR &= ~((1 << ISC01) | (1 << ISC00));
	
	
	
	// Timer 0 konfigurieren
	
	TCCR2A |= (1<<CS02);
	TCCR2A |= (1<<CS00); // Prescaler 1024

	
	// Overflow Interrupt erlauben
	TIMSK2 |= (1<<TOIE2);
	

	set_sleep_mode (SLEEP_MODE_ADC);
	
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
		counter=0x00100000; //0x00949000 entspricht einem Tag;
		sei();
		
		while(counter>=0x300){
			sleep_cpu();
	
			cli();
			counter--;
			sei();
		}
		
		//todo: Sensoren hochfahren
		
		
			while(counter>=0x200){
				sleep_cpu();
				
				cli();
				counter--;
				sei();
			}
			// PORTC
		
		if(1){ //genuegend Hellkeitvorhanden
			if(1){//Feuchtigkeit hoch genug
			PORTC |= (1 << PC1);//Ventil auf
			}
		}
		
		while(counter>=1){
			sleep_cpu();
			cli();
			counter--;
			sei();
		}
			
		
			
		
	}
}


/* ADC initialisieren */
void ADC_Init(void) {
 
  
  ADMUX = (1<<REFS0);    // die 5V als Referenz waehlen:
  
  ADCSRA = (1<<ADPS1) | (1<<ADPS0);     // Frequenzvorteiler
  ADCSRA |= (1<<ADEN);                  // ADC aktivieren
 
  /* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
     also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */
 
  ADCSRA |= (1<<ADSC);                  // eine ADC-Wandlung 
  while (ADCSRA & (1<<ADSC) ) {         // auf Abschluss der Konvertierung warten
  }
  /* ADCW muss einmal gelesen werden, sonst wird Ergebnis der nächsten
     Wandlung nicht übernommen. */
  (void) ADCW;
}


/* ADC Einzelmessung */
uint16_t ADC_Read( uint8_t channel )
{
	// Kanal waehlen, ohne andere Bits zu beeinflußen
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
	while (ADCSRA & (1<<ADSC) ) {   // Warten bis ADC-Wandlung abgeschlossen
		}
	return ADCW;  
}

/* Durschnittsberechnung aus den ADC_Read*/
uint16_t ADC_Read_Avg( uint8_t channel, uint8_t nsamples )
{
	
	uint16_t sum = 0;
	
	for (uint8_t i = 0; i < nsamples; ++i ) {
		cli();
		sum += ADC_Read( channel );
		sei();
	}
	cli();
	sum =( sum / nsamples );
	sei();
	return sum;
}

