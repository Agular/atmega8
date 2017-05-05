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
#include "i2cmaster.h"
#include <avr/sleep.h>

#define Dev24C32  0xA0
#define DS1621    0x90
/*
 * Delay in ms
 */
void delay_ms(unsigned short ms){
    for(ms /= 10; ms > 0; ms--)
        _delay_ms(10);
}

int main(void)
{   
    char buf[2];
    char buffer[128];
    DDRB = 1 << PIN0 | 1 << PIN1 | 1 << PIN2 | 1 << PIN3;                            

    i2c_init();
    lcd_init(LCD_DISP_ON);
    lcd_clrscr();
    
    i2c_start(DS1621 + I2C_WRITE);
    i2c_write(0xEE);
    i2c_stop();
    i2c_start_wait(DS1621 + I2C_WRITE);
    i2c_write(0xAA);
    i2c_rep_start(DS1621 + I2C_READ);
    buf[0] = i2c_readAck();
    buf[1] = i2c_readNak();
    i2c_stop();
    sprintf(buffer, "%d.%d\n", buf[0],buf[1]);
    lcd_puts(buffer);
    PORTB=1;
    return 0;
}