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
#define MEASURE_PER_SEC     4
#define counter         0x10000-(43200/MEASURE_PER_SEC)

uint16_t adc;
char buffer[128];


###########NEEDS ADC FUNCTION#############
###########ACCIDENTALLY DELETED###########

/*
 * Delay in ms
 */
void delay_ms(unsigned short ms){
    for(ms /= 10; ms > 0; ms--)
        _delay_ms(10);
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
void USART_Init(unsigned long ubrr){
    
    UBRRH = (unsigned char) (ubrr >> 8);
    UBRRL = (unsigned char) ubrr&0xff;
//  RXEN: Receive enable
//  TXEN: Transmit enable
    UCSRB = (1<<RXEN) |(1<<TXEN);

    UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); // 8N1

// Flush Receive-Buffer
    do
    {
       uint8_t dummy;
          (void) (dummy = UDR);
    }
    while (UCSRA & (1 << RXC));
}

void init(){
    USART_Init(UBRR);
    lcd_init(LCD_DISP_ON);
    /*Set output pins*/
    DDRB = 1 << PIN0 | 1 << PIN1 | 1 << PIN2 | 1 << PIN3;   // 1:out 0:in
    //Set the counter value
    TCNT1 = counter;
    //Set the register, B is responsible for activating the counter.
    TCCR1A = 0;
    TCCR1B = (1<<CS12);
    //Enable timer interrupts on Timer1 overflow.
    TIMSK = (1 << TOIE1);

    /*
     * Allow Global Interrupts
     */
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    cli();
}

ISR(TIMER1_OVF_vect){
    TCNT1 = counter;
    PORTB ^=1;
}



int main(){

    init();
        while(1){
            sleep_enable();
            sei();
            sleep_cpu();
            sleep_disable();
            lcd_clrscr();
            float result = (float)(adc + 1) * REFERENCE_VOL / 1024;
            int leds = (int) ((result / REFERENCE_VOL) * PIN_MAX);
            sprintf(buffer, "Vol: %.3f\nLED: %d\n", result, leds);
            uputs("##########\n");
            uputs(buffer);
            lcd_puts(buffer);
            PORTB = leds;
        }
    return 0;
}