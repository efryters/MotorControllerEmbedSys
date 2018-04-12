/*
 * lcd_functions.c
 *
 *  Created on: Feb 16, 2018
 *      Author: Eric Fryters
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "lcd_functions.h"
#include "../i2c/i2c_setup.h"
#include "delay.h"
#include <string.h>


/*
 * 1. init lcd in 4-bit mode (using full byte sends via i2c)
 * 2. after init, begin sending data via nibble mode.
 */
void setup_lcd(void)
{

    /*
     *        08   04   02   01   08   04   02   01
     *      +----+----+----+----+----+----+----+----+
     *      | D7 | D6 | D5 | D4 | BL | EN | RW | RS |
     * BIT7 +----+----+----+----+----+----+----+----+ BIT0
     *
     *          DATA BYTE MAPPINGS FOR LCD (i2c)
     */


    //LCD init begin
    i2c_send(LCD_ADDR, 1, 0x00);
    delayMs(15);

    i2c_send(LCD_ADDR, 2, 0x34, BACKLIGHT);
    delayUs(4500);

    i2c_send(LCD_ADDR, 2, 0x34, BACKLIGHT);
    delayUs(200);

    i2c_send(LCD_ADDR, 2, 0x34, BACKLIGHT);
    delayUs(100);
    i2c_send(LCD_ADDR, 2, 0x24, BACKLIGHT);

    //now in 4-bit interface mode...
    //function set, 2 lines, font set
    write_byte_4bit_mode(0x38, false);
    delayMs(5);
    //display off
    write_byte_4bit_mode(0x08, false);
    delayUs(50);

    //set entry mode (increment by 1 after data in, with no disp shift)
    write_byte_4bit_mode(0x06, false);
    delayUs(50);

    clear_screen();
    cursor_home();

    //write_byte_4bit_mode(0x10, false);
    //write_byte_4bit_mode(0x10, false);

    delayMs(15);

    //Clear screen and return cursor home. Twice for reliablilty.
}

/*
 * Register select RS, true to select CGRAM
 * Input is the 8-bit for D0-D7, as noted in datasheet for LCD
 */
void write_byte_4bit_mode(uint8_t byte, bool rs)
{

    uint8_t rs_en;

    //with backlight, always
    if (rs)
    {
        //select register
        rs_en = RS|EN|BACKLIGHT;
    }
    else
    {
        //dont select register
        rs_en = EN|BACKLIGHT;
    }

    //shift the lower nibble to the front, then apply the RS and En
    uint8_t lower = byte << 4;
    lower |= rs_en;

    //clear out the lower nibble, then apply the RS and En
    uint8_t upper = byte;
    upper &= 0xF0;
    upper |= rs_en;

    //send our mangled bytes to i2c, then close enable, but leave on backlight
    i2c_send(LCD_ADDR, 2, upper, BACKLIGHT);
    i2c_send(LCD_ADDR, 2, lower, BACKLIGHT);
    check_busy_flag();
    i2c_send(LCD_ADDR, 1, BACKLIGHT);
}

/*
 * Write a single character to the display
 */
void print_char_4bit_mode(const char *c)
{
    write_byte_4bit_mode((uint8_t) c, true);
}

/*
 * Write a string (char array) to the display at the current cursor position
 */
void print_string_4bit_mode(const char *c)
{
    int i = 0;
    int len = strlen(c);
    for (i = 0; i < len; i++)
    {
        write_byte_4bit_mode((uint8_t) *(c+i), true);
    }
}

/*
 * check busy flag not needed anymore... Replaced with a delay to insure that LCD MCU is not
 * ever in a busy state.
 */
void check_busy_flag()
{
    //read DB7, and if true, loop until
    //uint32_t read = ;
    delayUs(2000);
    //while((uint8_t)i2c_read(LCD_ADDR, 0x80) != 0x80) {}

}

/*
 * Some more LCD helper functions below... Performs as titled.
 */
void backlight_on()
{
    i2c_send(LCD_ADDR, 1, BACKLIGHT);
}

void backlight_off()
{
    i2c_send(LCD_ADDR, 1, 0x00);
}

void clear_screen()
{
    write_byte_4bit_mode(0x01, false);
    delayMs(20);
}
void cursor_home()
{
    write_byte_4bit_mode(0x02, false);
    delayMs(20);
}

/*
 * Sets the cursor position on the LCD with given parameters
 * Parameters:
 *      uint8_t row: zero-based index row selection
 *      uint8_t col: zero-based index column selection
 */
void set_cursor_pos(uint8_t row, uint8_t col)
{
    char address;

    if (row == 0)
        address = 0;
    else if (row==1)
        address = 0x40;
    else if (row==2)
        address = 0x14;
    else if (row==3)
        address = 0x54;
    else
        address = 0;

    address += col;

    write_byte_4bit_mode(0x80 | address, false);
    delayUs(45);

}

/*
 * Print a string to LCD with given layout
 * Parameters:
 * const char *c: string to print
 *             j: justification setting
 *                  0: left justified
 *                  1: center
 *                  2: right justified
 *           row: LCD row to print at
 */
void print_string_justify(const char *c, uint8_t j, uint8_t row)
{
    uint8_t len;
    if(j != 0)
    {
        len = strlen(c);
    }
    switch(j)
    {
    case 0: //left justify, normal print
        set_cursor_pos(row, 0);
        print_string_4bit_mode(c);
        break;
    case 1: //print center
        set_cursor_pos(row, (10 - (len/2)));
        print_string_4bit_mode(c);
        break;
    case 2:
        set_cursor_pos(row, (20 - len));
        print_string_4bit_mode(c);
        break;
    default:
        set_cursor_pos(row, 0);
        print_string_4bit_mode(c);
    }
    //set_cursor_pos(row+1,0);
    cursor_off();
}

/*
 * Playing around with ascii animations.
 * A choppy busy cursor. Could be improved.
 */
void print_busy_cursor(uint8_t row, uint8_t col)
{
    char busy[4] = { '/', '-' , 0x5C, '|'};
    int i;
    for(i = 0; i < 4; i++)
    {
    set_cursor_pos(row, col);
    print_char_4bit_mode(busy[i]);
    delayMs(150);
    }
}

/*
 * As named, turns cursor on/off
 */
void cursor_on()
{
    write_byte_4bit_mode(0x0F, false);
}
void cursor_off()
{
    write_byte_4bit_mode(0x0C, false);
}

/*
 * Given a row, this function simply clears the line on the LCD
 * or in other words, overwrites with ' ' / SPACES
 */
void clear_line(uint8_t row)
{
    uint8_t i;
    set_cursor_pos(row, 0);
    for (i = 0; i<20; i++)
    {
        print_char_4bit_mode(' ');
    }
}

