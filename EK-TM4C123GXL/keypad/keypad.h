#ifndef KEYPAD_H_
#define KEYPAD_H_

// Truth Table
#define KP_ZERO 0x22
#define KP_ONE 0x14
#define KP_TWO 0x12
#define KP_THREE 0x18
#define KP_FOUR 0x84
#define KP_FIVE 0x82
#define KP_SIX 0x88
#define KP_SEVEN 0x44
#define KP_EIGHT 0x42
#define KP_NINE 0x48
#define KP_ASTERISK 0x24
#define KP_POUND 0x28

// Pin defines
#define ROW_PINS GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
#define ROW_BASE GPIO_PORTC_BASE
#define COL_PINS GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
#define COL_BASE GPIO_PORTE_BASE

// Method Prototype
void init_keypad(void);
uint8_t scan_keypad();
char get_input();
char get_char(uint8_t key_code);
uint16_t get_ct_input(uint8_t justify, uint8_t row, bool password);



#endif  /* KEYPAD_H_  */
