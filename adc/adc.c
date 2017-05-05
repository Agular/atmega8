/*************************************************************************
Title:    testing output to a HD44780 based LCD display.
Author:   Peter Fleury  <pfleury@gmx.ch>  http://jump.to/fleury
File:     $Id: test_lcd.c,v 1.6 2004/12/10 13:53:59 peter Exp $
Software: AVR-GCC 3.3
Hardware: HD44780 compatible LCD text display
          ATS90S8515/ATmega if memory-mapped LCD interface is used
          any AVR with 7 free I/O pins if 4-bit IO port mode is used
**************************************************************************/
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lcd.h"
#include <util/delay.h>
#include <stdio.h>

#define CHANNEL 5

uint16_t readADC(uint8_t channel) {
	uint8_t i;
	uint16_t result = 0;
	
	// Den ADC aktivieren und Teilungsfaktor auf 128 stellen
	ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0) ;

	// Kanal des Multiplexers waehlen
	ADMUX = (1<<MUX2) | (1<<MUX0) |(1<<REFS0);
	
	// Den ADC initialisieren und einen sog. Dummyreadout machen
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
            
	// Jetzt 3x die analoge Spannung and Kanal channel auslesen
	// und dann Durchschnittswert ausrechnen.
	for(i=0; i<3; i++) {
		// Eine Wandlung
		ADCSRA |= (1<<ADSC);
		// Auf Ergebnis warten...
		while(ADCSRA & (1<<ADSC));
		
		result += ADCW;
	}
	
	// ADC wieder deaktivieren
	ADCSRA &= ~(1<<ADEN);
	
	result /= 3;
	
	return result;
}


void delay_ms(unsigned short ms){
    for(ms /= 10; ms > 0; ms--)
        _delay_ms(10);
}

void init(){
    /*Set output pins*/
    DDRB = 1 << PIN0 | 1 << PIN1 | 1 << PIN2 | 1 << PIN3;   // 1:out 0:in
}

int main(void) {
    init();
     
    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);

    while(1){
        char buffer[16];
        lcd_clrscr();         
        uint16_t value = readADC(CHANNEL);
        sprintf(buffer, "%u", value);
        lcd_puts(buffer);
        lcd_gotoxy(0,1);
        float calculated = (float)(5 * (value+1) ) / 1024;
        sprintf(buffer, "%.3f", calculated);
        lcd_puts(buffer);
        PORTB ^= 1;
        delay_ms(1000);
    }
        
        
    }
