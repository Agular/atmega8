

//#define F_CPU 1000000UL         // Clock speed - besser aus Makefile mit -DFCPU=1000000

#include <avr/io.h>             /* defines für IO-Register/-pins */
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define keypress_dly    100
#define counter         0x10000-15625

typedef unsigned char BYTE;
typedef unsigned short WORD;

/*Global count variable*/
BYTE count = 0;

/*Interrupt handling from keyboard 0*/
ISR(TIMER1_OVF_vect){
    count++;
    PORTB ^= 1;
    TCNT1 = counter;
}

void init(){
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