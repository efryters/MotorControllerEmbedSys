#ifndef LCDFUNCT_H_
#define LCDFUNCT_H_



#define LCD_ADDR 0x27
#define EN 0x04
#define RS 0x01
#define BACKLIGHT 0x08


//Common LCD chars for functions
#define UNDERSCORE 0x5f


//Methods Implemented
void setup_lcd(void);
void print_char_4bit_mode(const char *c);
void print_string_4bit_mode(const char *c);
void write_byte_4bit_mode(uint8_t byte, bool rs);
void check_busy_flag();
void backlight_on();
void clear_screen();
void cursor_home();
void set_cursor_pos(uint8_t row, uint8_t col);
void print_string_justify(const char *c, uint8_t j, uint8_t row);
void print_busy_cursor(uint8_t row, uint8_t col);
void cursor_on();
void cursor_off();
void clear_line(uint8_t row);

//Methods not implemented yet
uint8_t get_cursor_pos();
void backlight_off();


#endif  /* LCDFUNCT_H_  */
