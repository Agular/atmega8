//#define F_CPU 1000000UL         // Clock speed - besser aus Makefile mit -DFCPU=1000000

#include <avr/io.h>             /* defines für IO-Register/-pins */
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define keypress_dly    100
#define key_mask        0x0c

typedef unsigned char BYTE;
typedef unsigned short WORD;

/*Global count variable*/
BYTE count = 0;

void delay_ms(unsigned short ms){
    for(ms /= 10; ms > 0; ms--)
        _delay_ms(10);
}

/*Interrupt handling from keyboard 0*/
ISR(INT0_vect){
    delay_ms(100);
    count++;
    PORTB = count;
}

/*Interrupt handling from keyboard 0*/
ISR(INT1_vect){
    delay_ms(100);
    count++;
    PORTB = count;
}

void init(){
    /*Set output pins*/
    DDRB = 1 << PIN0 | 1 << PIN1 | 1 << PIN2 | 1 << PIN3;   // 1:out 0:in
    MCUCR |= (1<<ISC11);
    GIMSK = (1<<INT0)|(1<<INT1);

    /*
     * Allow Interrupts
     */
    sei();
}

int main(){
    
    init();
    while(1){
        
    }
    return 0;
}