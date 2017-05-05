/*************************************************************************
Title:    Analog-Digital converter that sends data to the computer. Works at the moment with a light-sensitive resistor.
Author:   Ragnar Luga
**************************************************************************/

#include <avr/io.h>             /* defines für IO-Register/-pins */
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcd.h"

#define BAUD 9600
#define UBRR ((F_CPU/(16UL*BAUD))-1)
#define user_delay          250
#define REFERENCE_VOL       5
#define PIN_MAX             15
/*
 * Delay in ms
 */
void delay_ms(unsigned short ms){
    for(ms /= 10; ms > 0; ms--)
        _delay_ms(10);
}


void USART_Init(unsigned long ubrr){
    
    UBRRH = (unsigned char) (ubrr >> 8);
    UBRRL = (unsigned char) ubrr&0xff;
// Aktivieren von receiver und transmitter
// RXEN Receive enable
// TXEN Transmit enable
// RXCIE Receive complete Interrupt enable
    UCSRB = (1<<RXEN) |(1<<TXEN);//|(1<<RXCIE);

/* Einstellen des Datenformats: 8 Datenbits, 1 Stoppbit, keine Paritätskontrolle */
    UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); // 8N1

// Flush Receive-Buffer (entfernen evtl. vorhandener ungültiger Werte)
    do
    {
       uint8_t dummy;
          (void) (dummy = UDR);
    }
    while (UCSRA & (1 << RXC));
}
void init(){
    USART_Init(UBRR);
    /*Initialize the LCD display*/
    lcd_init(LCD_DISP_ON);
    
    /*Set output pins*/
    DDRB = 1 << PIN0 | 1 << PIN1 | 1 << PIN2 | 1 << PIN3;   // 1:out 0:in
    
    /* Set the multiplexer to listen from the 5th channel, set REFS0 so internal reference is used*/
    ADMUX = (1<<MUX2) | (1<<MUX0) |(1<<REFS0);
    
    /*Activate the ADC, set it on Free Running mode, set the frequency modifier, enable interrupts*/
    /*NB! Even when activating interrupts (and Free Running mode), user needs to start the measuring himself with ADSC !!!*/
    
    ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADFR) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS0);
    
    /*Activate global interrupts*/
    sei();
}

void USART_Transmit(unsigned char data){
    
    while(! (UCSRA & (1<<UDRE)));
    UDR = data;
}

void uputs(char *s){
    while(*s){
        USART_Transmit(*s++);
    }
}


/*
 ADC_vect interrupt.
 This takes care of measuring the resistance in the light-sensitive resistor.
 TIP: Make the interrupt smaller. The display of the result should be in the main loop.
 Save the result in an extern variable and then display the result.
 */
ISR(ADC_vect){
    lcd_clrscr();
    float result = (float)(ADCW + 1) * REFERENCE_VOL / 1024;
    char buffer[10];
    sprintf(buffer, "V: %.3f\n", result);
    uputs(buffer);
    lcd_puts(buffer);
    int leds = (int) ((result / REFERENCE_VOL) * PIN_MAX);
    PORTB = leds;
    sprintf(buffer, "LED: %d\n", leds);
    lcd_puts(buffer);
    delay_ms(user_delay);
}


int main(){
    init();
            while(1){
 
            }
        
        return 0;
}