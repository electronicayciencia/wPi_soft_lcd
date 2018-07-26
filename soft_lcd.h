#ifndef _SOFT_LCD_H
#define _SOFT_LCD_H

#include "soft_i2c.h"

#define LCD_RS      1
#define LCD_READ    2
#define LCD_WRITE   0
#define LCD_ENABLED 4
#define LCD_BKLIGHT 8

#define LCD_D0      1
#define LCD_D1      2
#define LCD_D2      4
#define LCD_D3      8
#define LCD_D4      10
#define LCD_D5      20
#define LCD_D6      40
#define LCD_D7      80

#define LCD_CMD_DDRAM_SET LCD_D7
#define LCD_CMD_CGRAM_SET LCD_D6

#define LCD_CMD_FCN_SET        LCD_D5
#define LCD_FCN_4BIT           0
#define LCD_FCN_8BIT           LCD_D4
#define LCD_FCN_1LINE          0
#define LCD_FCN_2LINES         LCD_D3
#define LCD_FCN_5x8            0
#define LCD_FCN_5x10           LCD_D2

#define LCD_CMD_CURSOR_SET     LCD_D4
#define LCD_CURSOR_MOVE_CUR    0
#define LCD_CURSOR_SHIFT       LCD_D3
#define LCD_CURSOR_LEFT        0
#define LCD_CURSOR_RIGHT       LCD_D2

#define LCD_CMD_DISPLAY_SET    LCD_D3
#define LCD_DISPLAY_OFF        0
#define LCD_DISPLAY_ON         LCD_D2
#define LCD_DISPLAY_CURSOR_OFF 0
#define LCD_DISPLAY_CURSOR_ON  LCD_D1
#define LCD_DISPLAY_BLINK_OFF  0
#define LCD_DISPLAY_BLINK_ON   LCD_D0

#define LCD_CMD_HOME  LCD_D1
#define LCD_CMD_CLEAR LCD_D0





typedef struct {
	int backlight;
	int _addr;
	i2c_t _i2c;
} lcd_t;

int _pcf8874_check (lcd_t *lcd);
void lcd_reset (lcd_t *lcd);
int _pcf_put(lcd_t *lcd, int lines);
void lcd_raw (lcd_t *lcd, int lcd_opts, int data);
void lcd_home (lcd_t *lcd);
void lcd_clear (lcd_t *lcd);


/* Tries to initialize LCD via I2C */
lcd_t *lcd_init(int scl, int sda, int addr);

#endif

