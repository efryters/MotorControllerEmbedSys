#ifndef KEYPAD_H_
#define KEYPAD_H_

// Truth Table
#define KP_ZERO 0x21
#define KP_ONE 0x48
#define KP_TWO 0x28
#define KP_THREE 0x18
#define KP_FOUR 0x44
#define KP_FIVE 0x24
#define KP_SIX 0x14
#define KP_SEVEN 0x42
#define KP_EIGHT 0x22
#define KP_NINE 0x12
#define KP_ASTERISK 0x41
#define KP_POUND 0x11

// Method Prototype
void init_port(void);
void define_pins(void);






#endif  /* KEYPAD_H_  */
