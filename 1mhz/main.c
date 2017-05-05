//#define F_CPU 1000000UL         // Clock speed - besser aus Makefile mit -DFCPU=1000000

#include <avr/io.h>             /* defines für IO-Register/-pins */
#include <avr/pgmspace.h>
#include <util/delay.h>


/*
 * Delay in ms
 */
void delay_ms(unsigned short ms){
    for(ms /= 10; ms > 0; ms--)
        _delay_ms(10);
}

int main(){
        DDRB = 1 << PIN0 | 1 << PIN1 | 1 << PIN2 | 1 << PIN3;   // 1:out 0:in
        while(1){
            PORTB = 1 << PIN0;
            delay_ms(1000);
            PORTB &= ~(1 << PIN0);
            PORTB = 1 << PIN1;
            delay_ms(1000);
            PORTB &= ~(1 << PIN1);
            PORTB = 1 << PIN2;
            delay_ms(1000);
            PORTB &= ~(1 << PIN2);
            PORTB = 1 << PIN3;
            delay_ms(1000);
            PORTB &= ~(1 << PIN3);
        }
        return 0;
}