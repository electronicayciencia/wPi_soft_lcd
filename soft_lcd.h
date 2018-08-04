/*
 * wPi_soft_lcd
 *      Library to use PCF8574 based LCD via software I2C bus.
 *      using WiringPi library GPIO.
 *
 *      Electronica y ciencia.
 *      https://github.com/electronicayciencia/wPi_soft_lcd
 *      https://electronicayciencia.blogspot.com/
 *
 *      Reinoso G.
 */

 
#ifndef _SOFT_LCD_H
#define _SOFT_LCD_H

#include "soft_i2c.h"

#define LCD_RS      1
#define LCD_READ    2
#define LCD_WRITE   0
#define LCD_ENABLED 4
#define LCD_BKLIGHT 8

#define LCD_BUSY_FLAG 0x80

#define LCD_D0      0x01
#define LCD_D1      0x02
#define LCD_D2      0x04
#define LCD_D3      0x08
#define LCD_D4      0x10
#define LCD_D5      0x20
#define LCD_D6      0x40
#define LCD_D7      0x80

#define LCD_CMD_DDRAM_SET      LCD_D7
#define LCD_CMD_CGRAM_SET      LCD_D6

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

#define LCD_CMD_ENTRYMODE_SET       LCD_D2
#define LCD_ENTRYMODE_CURSOR_DECR   0
#define LCD_ENTRYMODE_CURSOR_INCR   LCD_D1
#define LCD_ENTRYMODE_SCROLL_OFF    0
#define LCD_ENTRYMODE_SCROLL_ON     LCD_D0

#define LCD_CMD_HOME           LCD_D1
#define LCD_CMD_CLEAR          LCD_D0

#define LCD_ERR 1
#define LCD_ERR_I2C 1

/* PWM pins are WPi 1 and WPi 26 */
#define LCD_PWM_PIN     1
#define LCD_PWM_CLOCK   19
#define LCD_PWM_RANGE   1024


typedef struct {
	/* LCD configuration */
	int fcn_set;
	int cursor_set;
	int display_set;
	int entrymode_set;

	/* User options */
	int backlight;
	int replace_UTF8_chars;
	int err;

	/* Internals */
	int _lines;
	int _addr;
	float _dimming;
	i2c_t _i2c;
} lcd_t;

lcd_t *lcd_create(int scl, int sda, int addr, int lines);
void lcd_destroy(lcd_t *lcd);
void lcd_init(lcd_t *lcd);

void lcd_printf(lcd_t *lcd, const char* format, ... );
void lcd_print(lcd_t *lcd, char *s);
void lcd_pos(lcd_t *lcd, int row, int col);
void lcd_home(lcd_t *lcd);
void lcd_clear(lcd_t *lcd);
void lcd_on(lcd_t *lcd);
void lcd_off(lcd_t *lcd);
void lcd_backlight_on(lcd_t *lcd);
void lcd_backlight_off(lcd_t *lcd);
void lcd_backlight_dim(lcd_t *lcd, float intensity);
void lcd_cursor_on(lcd_t *lcd);
void lcd_cursor_off(lcd_t *lcd);
void lcd_blink_on(lcd_t *lcd);
void lcd_blink_off(lcd_t *lcd);
void lcd_create_char(lcd_t *lcd, int n, char *data);

void lcd_reconfig(lcd_t *lcd);
void lcd_reconfig_fcn(lcd_t *lcd);
void lcd_reconfig_cursor(lcd_t *lcd);
void lcd_reconfig_display(lcd_t *lcd);
void lcd_reconfig_entrymode(lcd_t *lcd);

void lcd_pos_raw (lcd_t *lcd, int pos);
void lcd_reset (lcd_t *lcd);
void lcd_raw (lcd_t *lcd, int lcd_opts, int data);

int lcd_read_pos_raw (lcd_t *lcd);
int lcd_read_data (lcd_t *lcd);
int lcd_read_raw (lcd_t *lcd, int rs);

void _lcd_nextline(lcd_t *lcd);
char *_replace_UTF8_chars(char *s);

void _pcf8574_put (lcd_t *lcd, int lines);
int _pcf8574_get (lcd_t *lcd, int rs);
int _pcf8574_check (i2c_t i2c, int addr);

#endif

