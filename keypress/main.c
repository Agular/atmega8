//#define F_CPU 1000000UL         // Clock speed - besser aus Makefile mit -DFCPU=1000000

#include <avr/io.h>             /* defines für IO-Register/-pins */
#include <avr/pgmspace.h>
#include <util/delay.h>

#define keypress_dly    100
#define key_mask        0x0c

typedef unsigned char BYTE;
typedef unsigned short WORD;

/*
 * Delay in ms
 */
void delay_ms(unsigned short ms){
    for(ms /= 10; ms > 0; ms--)
        _delay_ms(10);
}

BYTE keypress(){
    BYTE temp1;
    temp1 = ~PIND;
    if(!(temp1 & key_mask))
        return 0;
    delay_ms(keypress_dly);
    temp1 &= ~PIND & key_mask;
    while(~PIND & key_mask);
    return temp1;
}

int main(){
    BYTE key, count = 0;
        DDRB = 1 << PIN0 | 1 << PIN1 | 1 << PIN2 | 1 << PIN3;   // 1:out 0:in
//         PORTD |= key_mask;      // intern pull-ups
        for(;;){
            key = keypress();
            if(key == (1<< PIN2)){
            
                count++;
            } else if (key == (1 << PIN3)){
            
            count = 0;
            }
            PORTB = count;
        }
        return 0;
}