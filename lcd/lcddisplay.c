/*************************************************************************
Title:    testing output to a HD44780 based LCD display.
Author:   Peter Fleury  <pfleury@gmx.ch>  http://jump.to/fleury
File:     $Id: test_lcd.c,v 1.6 2004/12/10 13:53:59 peter Exp $
Software: AVR-GCC 3.3
Hardware: HD44780 compatible LCD text display
          ATS90S8515/ATmega if memory-mapped LCD interface is used
          any AVR with 7 free I/O pins if 4-bit IO port mode is used
**************************************************************************/
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "lcd.h"
#include <util/delay.h>
#include <avr/eeprom.h>
/*
** constant definitions
*/
static const PROGMEM unsigned char copyRightChar[] =
{
	0x07, 0x08, 0x13, 0x14, 0x14, 0x13, 0x08, 0x07,
	0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x00
};


/*
** function prototypes
*/ 
void wait_until_key_pressed(void);


void wait_until_key_pressed(void)
{
    unsigned char temp1, temp2;
    unsigned int i;
    
    do {
        temp1 = PIND;                  // read input
        for(i=0;i<65535;i++);
        temp2 = PIND;                  // read input
        temp1 = (temp1 & temp2);       // debounce input
    } while ( temp1 & _BV(PIND2) );
    
    loop_until_bit_is_set(PIND,PIND2);            /* wait until key is released */
}
void delay_ms(unsigned short ms){
    for(ms /= 10; ms > 0; ms--)
        _delay_ms(10);
}

int main(void)
{
    char *ps1 = PSTR("fun forever");
    
    
    DDRD &=~ (1 << PD2);        /* Pin PD2 input              */
    PORTD |= (1 << PD2);        /* Pin PD2 pull-up enabled    */


    /* initialize display, cursor off */
    lcd_init(LCD_DISP_ON);

    for (;;) {
        int i, j;
        /*Write from RAM*/
        /*Loading screen */
        for(i=0;i<11;i++){
            lcd_clrscr();
            lcd_puts("Loading\n");
            lcd_puts("Rdy:|");
            for(j= 0; j<i; j++){
                lcd_putc('#');
            }
            lcd_gotoxy(15,1);
            lcd_putc('|');
            delay_ms(500);
        }
        
        /*Write from Flash Memory*/
        lcd_clrscr();
        lcd_puts_p(ps1);
        delay_ms(1000);
        
        
//         /*Read from EEPROM*/
//         lcd_clrscr();
//         uint8_t Byte;
//         uint16_t Word;
//         Byte = eeprom_read_byte((uint8_t*) 46);
//         Word = eeprom_read_word((uint16_t*)46);
//         lcd_putc(Byte);
//         delay_ms(1000);
//         lcd_puts(Word);
//         delay_ms(1000);
    }
}
