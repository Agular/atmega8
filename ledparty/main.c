//#define F_CPU 1000000UL         // Clock speed - besser aus Makefile mit -DFCPU=1000000

#include <avr/io.h>             /* defines für IO-Register/-pins */
#include <avr/pgmspace.h>
#include <util/delay.h>
/*This is meant for fun practices.*/
#define DELTA_TIME 100



/*
 * Delay in ms
 */
void delay_ms(unsigned short ms){
    for(ms /= 10; ms > 0; ms--)
        _delay_ms(10);
}
/* Original code*/
void l_to_r(){
    PORTB = 1 << PIN0;
    delay_ms(DELTA_TIME);
    PORTB &= ~(1 << PIN0);
    PORTB = 1 << PIN1;
    delay_ms(DELTA_TIME);
    PORTB &= ~(1 << PIN1);
    PORTB = 1 << PIN2;
    delay_ms(DELTA_TIME);
    PORTB &= ~(1 << PIN2);
    PORTB = 1 << PIN3;
    delay_ms(DELTA_TIME);
    PORTB &= ~(1 << PIN3);    
}
void r_to_l(){
    PORTB = 1 << PIN3;
    delay_ms(DELTA_TIME);
    PORTB &= ~(1 << PIN3);
    PORTB = 1 << PIN2;
    delay_ms(DELTA_TIME);
    PORTB &= ~(1 << PIN2);
    PORTB = 1 << PIN1;
    delay_ms(DELTA_TIME);
    PORTB &= ~(1 << PIN1);
    PORTB = 1 << PIN0;
    delay_ms(DELTA_TIME);
    PORTB &= ~(1 << PIN0);    
}
/*Binary count from 0 to 15*/
void count_to_15(){
    int i;
    for(i = 0; i<16; i++){
        PORTB = i;
        delay_ms(DELTA_TIME);
    }
}

void ll_to_rr(){
    int i;
    for (i= 0; i<4; i++){
        PORTB = 5;
        delay_ms(DELTA_TIME);
        PORTB = 10;
        delay_ms(DELTA_TIME);    
    }
}

void rr_to_ll(){
    int i;
    for (i= 0; i<4; i++){
        PORTB = 10;
        delay_ms(DELTA_TIME);
        PORTB = 5;
        delay_ms(DELTA_TIME);    
    }
}

void lll_to_rrr(){
    int i;
    for (i= 0; i<4; i++){
        PORTB = 1;
        delay_ms(DELTA_TIME);
        PORTB = 3;
        delay_ms(DELTA_TIME);
        PORTB = 7;
        delay_ms(DELTA_TIME);
        PORTB = 14;
        delay_ms(DELTA_TIME);
        PORTB = 12;
        delay_ms(DELTA_TIME);
        PORTB = 8;
        delay_ms(DELTA_TIME);
    }
}

void rrr_to_lll(){
    int i;
    for (i= 0; i<4; i++){
        PORTB = 8;
        delay_ms(DELTA_TIME);
        PORTB = 12;
        delay_ms(DELTA_TIME);
        PORTB = 14;
        delay_ms(DELTA_TIME);
        PORTB = 7;
        delay_ms(DELTA_TIME);
        PORTB = 3;
        delay_ms(DELTA_TIME);
        PORTB = 1;
        delay_ms(DELTA_TIME);
    }
}

void out_to_in(){
        int i;
    for (i= 0; i<4; i++){
        PORTB = 9;
        delay_ms(DELTA_TIME * 2);
        PORTB = 6;
        delay_ms(DELTA_TIME * 2);    
    }
}

void in_to_out(){
        int i;
    for (i= 0; i<4; i++){
        PORTB = 6;
        delay_ms(DELTA_TIME * 2);
        PORTB = 9;
        delay_ms(DELTA_TIME * 2);    
    }
}
    
int main(){
        DDRB = 1 << PIN0 | 1 << PIN1 | 1 << PIN2 | 1 << PIN3;   // 1:out 0:in
        while(1){
            r_to_l();
            rr_to_ll();            
            l_to_r();
            ll_to_rr();
            r_to_l();
            r_to_l();
            out_to_in();
            l_to_r();
            l_to_r();
            in_to_out();
            l_to_r();
            l_to_r();
            lll_to_rrr();
            r_to_l();
            r_to_l();
            rrr_to_lll();
        }
        return 0;
}