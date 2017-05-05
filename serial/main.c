//#define F_CPU 1000000UL         // Clock speed - besser aus Makefile mit -DFCPU=1000000

#include <avr/io.h>             /* defines für IO-Register/-pins */
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD 9600
#define UBRR ((F_CPU/(16UL*BAUD))-1)
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

int main(){
    USART_Init(UBRR);
        DDRB = 1 << PIN0 | 1 << PIN1 | 1 << PIN2 | 1 << PIN3;   // 1:out 0:in
        
            PORTB = 1 << PIN0;
            delay_ms(500);
            
            PORTB = 0;
            
            uputs("Start\n");
            char count = '0';
            while(1){
                USART_Transmit(count);
                USART_Transmit('\n');
                count = count >= '9' ? '0':count + 1;
                delay_ms(500);
            }
        
        ////return 0;
}