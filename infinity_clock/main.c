#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "i2cmaster.h"


/*
### INFINITY CLOCK ###

This is the code for the Infinity Clock project. The previous code was
accidentally deleted, so I will make sure to make backups this time.
The code runs on ATMEGA-16PU, that controls three IO-Extenders PCF8574.
There are 23 LEDs in total that are shaped as an endless loop. 
 */






#define counter         0x10000-15625


/*Interrupt handling from keyboard 0*/
ISR(TIMER1_OVF_vect){
    
}

void init(){
    i2c_init();
    
    /*Set output pins*/
    DDRB = 1 << PIN0 | 1 << PIN1 | 1 << PIN2 | 1 << PIN3;   // 1:out 0:in
    //Set the counter value
    TCNT1 = counter;
    //Set the register, B is responsible for activating the counter.
    TCCR1A = 0;
    TCCR1B = (1<<CS11) | (1<<CS10);
    //Enable timer interrupts on Timer1 overflow.
    TIMSK = (1 << TOIE1);

    /*
     * Allow Global Interrupts
     */
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    cli();
    
}

int main(){
    
    init();
    while(1){
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
    }
    return 0;
}