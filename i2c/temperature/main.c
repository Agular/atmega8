/*************************************************************************
Title:    Temperature sensor.
Author:   Ragnar Luga
**************************************************************************/

#include <avr/io.h>             /* defines for IO-Register/-pins */
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcd.h"
#include "i2cmaster.h"
#include <avr/sleep.h>

#define SENSOR    0x90
#define BAUD 9600
#define UBRR ((F_CPU/(16UL*BAUD))-1)
#define MEASURE_PER_SEC     2
#define counter         0x10000-(43200/MEASURE_PER_SEC)

char buffer[128];
int temperature;
int half;

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
    i2c_init();
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

    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    cli();
    
    /*Activate continuous conversion on temperature sensor*/
    /*Note: By default, DS1621 is in continuous mode, we  */
    /*just activate it to start the conversions           */
    i2c_start(SENSOR + I2C_WRITE);
    i2c_write(0xEE);
    i2c_stop();
}

void readSensorT(){
    i2c_start_wait(SENSOR + I2C_WRITE);
    i2c_write(0xAA);
    i2c_rep_start(SENSOR + I2C_READ);
    temperature = i2c_readAck();
    half = i2c_readNak();
    i2c_stop();
}

ISR(TIMER1_OVF_vect){
    TCNT1 = counter;
    readSensorT();
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
            /*223 is the ascii character for the small o*/
            sprintf(buffer, "T: %d.%d C\n", temperature,half/128 * 5);
            uputs("##########\n");
            uputs(buffer);
            lcd_puts(buffer);
        }
    return 0;
}