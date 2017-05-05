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
#include <avr/sleep.h>

#define BAUD 9600
#define UBRR ((F_CPU/(16UL*BAUD))-1)
#define REFERENCE_VOL       5
#define PIN_MAX             15
#define MEASURE_PER_SEC     2
#define counter             0x10000-(43200/MEASURE_PER_SEC)
#define RING_SIZE           8
uint16_t adc;
unsigned char buffer[RING_SIZE];
volatile unsigned short irr=0; 
volatile unsigned short irw=0;

/* Interrupt for USART, Rx Complete */
ISR(USART_RXC_vect) {
    buffer[irw] = UDR;
    irw = (irw+1) % RING_SIZE;
}


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
    UCSRB = (1<<RXEN) |(1<<TXEN)|(1<<RXCIE);

/* Einstellen des Datenformats: 8 Datenbits, 1 Stoppbit, keine Paritätskontrolle */
    UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); // 8N1

// Flush Receive-Buffer (entfernen evtl. vorhandener ungültiger Werte)
    do
    {
       uint8_t dummy;
          (void) (dummy = UDR);
    }
    while (UCSRA & (1 << RXC));
    sei();
}

void init(){
    USART_Init(UBRR);
    lcd_init(LCD_DISP_ON);
    /*Set output pins*/
    DDRB = 1 << PIN0 | 1 << PIN1 | 1 << PIN2 | 1 << PIN3;   // 1:out 0:in
}

int main(){
    char buff[10];
    init();
        while(1){
            char c = ugetchar();
            sprintf(buff, "%c", c);
            lcd_puts(buff);
            delay_ms(1000);
        }
    return 0;
}