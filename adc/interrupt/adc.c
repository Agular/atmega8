/*************************************************************************
Title:    Analog-Digital converter. Works at the moment with a light-sensitive resistor.
Author:   Ragnar Luga
**************************************************************************/
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lcd.h"
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define user_delay          250
#define REFERENCE_VOL       5
#define PIN_MAX             15
void init(){
    
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

void delay_ms(unsigned short ms){
    for(ms /= 10; ms > 0; ms--)
        _delay_ms(10);
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
    sprintf(buffer, "V: %.3f", result);
    lcd_puts(buffer);
    int leds = (int) ((result / REFERENCE_VOL) * PIN_MAX);
    PORTB = leds;
    lcd_gotoxy(0,1);
    sprintf(buffer, "LED: %d", leds);
    lcd_puts(buffer);
    delay_ms(user_delay);
}

int main(void) {
    init();
    /*For loop*/
    while(1){}
}
