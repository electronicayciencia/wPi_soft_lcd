#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include "soft_lcd.h"
#include "soft_i2c.h"


lcd_t *lcd_create(int scl, int sda, int addr, int lines) {

	if (wiringPiSetup () == -1)
	        return NULL;
	
	i2c_t i2c = i2c_init(scl, sda);
	if ( !_pcf8574_check(i2c, addr) ) return NULL;

	lcd_t *lcd = (lcd_t*) malloc(sizeof(lcd_t));

	lcd->_addr         = addr;
	lcd->_i2c          = i2c_init(scl, sda);
	lcd->err           = 0;

	lcd->fcn_set       = LCD_FCN_4BIT | LCD_FCN_5x8;

	if (lines > 1)
		 lcd->fcn_set |= LCD_FCN_2LINES;

	lcd->cursor_set    = LCD_CURSOR_MOVE_CUR | LCD_CURSOR_LEFT;
	lcd->display_set   = LCD_DISPLAY_ON | LCD_DISPLAY_CURSOR_OFF | LCD_DISPLAY_BLINK_OFF;
	lcd->entrymode_set = LCD_ENTRYMODE_CURSOR_DECR | LCD_ENTRYMODE_SCROLL_OFF;

	lcd->backlight     = LCD_BKLIGHT;
	lcd->replace_UTF8_chars = 1;
	
	lcd_reset(lcd);
	lcd_init(lcd);

	return lcd;
}

void lcd_destroy(lcd_t *lcd) {
	lcd_off(lcd);
	lcd_backlight_off(lcd);
	lcd_reset(lcd);
	free(lcd);
}


void lcd_init(lcd_t *lcd) {
	lcd_reconfig(lcd);
	lcd_clear(lcd);
	lcd_home(lcd);
}


/* send configuration parameters to LCD */
void lcd_reconfig(lcd_t *lcd) {
	lcd_reconfig_fcn(lcd);
	lcd_reconfig_cursor(lcd);
	lcd_reconfig_display(lcd);
	lcd_reconfig_entrymode(lcd);
}

void lcd_reconfig_fcn(lcd_t *lcd) { 
	lcd_raw(lcd, LCD_WRITE, LCD_CMD_FCN_SET | lcd->fcn_set);
}
void lcd_reconfig_cursor(lcd_t *lcd) {
	lcd_raw(lcd, LCD_WRITE, LCD_CMD_CURSOR_SET | lcd->cursor_set);
}
void lcd_reconfig_display(lcd_t *lcd) {
	lcd_raw(lcd, LCD_WRITE, LCD_CMD_DISPLAY_SET | lcd->display_set);
}
void lcd_reconfig_entrymode(lcd_t *lcd) {
	lcd_raw(lcd, LCD_WRITE, LCD_CMD_ENTRYMODE_SET | lcd->entrymode_set);
}

void lcd_home (lcd_t *lcd) {
	lcd_raw(lcd, LCD_WRITE, LCD_CMD_HOME);
}

void lcd_clear (lcd_t *lcd) {
	lcd_raw(lcd, LCD_WRITE, LCD_CMD_CLEAR);
	usleep(2000);
}

/* Convenient shortcuts */
void lcd_on(lcd_t *lcd) {
	lcd->display_set |= LCD_DISPLAY_ON;
	lcd_reconfig_display(lcd);
}

void lcd_off(lcd_t *lcd) {
	lcd->display_set &= ~LCD_DISPLAY_ON;
	lcd_reconfig_display(lcd);
}

void lcd_backlight_on(lcd_t *lcd) {
	lcd->backlight = LCD_BKLIGHT;
	lcd_reconfig_display(lcd);
}

void lcd_backlight_off(lcd_t *lcd) {
	lcd->backlight = 0;
	lcd_reconfig_display(lcd);
}

void lcd_cursor_on(lcd_t *lcd) {
	lcd->display_set |= LCD_DISPLAY_CURSOR_ON;
	lcd_reconfig_display(lcd);
}

void lcd_cursor_off(lcd_t *lcd) {
	lcd->display_set &= ~LCD_DISPLAY_CURSOR_ON;
	lcd_reconfig_display(lcd);
}

void lcd_blink_on(lcd_t *lcd) {
	lcd->display_set |= LCD_DISPLAY_BLINK_ON;
	lcd_reconfig_display(lcd);
}

void lcd_blink_off(lcd_t *lcd) {
	lcd->display_set &= ~LCD_DISPLAY_BLINK_ON;
	lcd_reconfig_display(lcd);
}

void lcd_pos(lcd_t *lcd, int row, int col) {
	int r_value[] = {0x00, 0x40, 0x14, 0x54};
	lcd_raw(lcd, LCD_WRITE, LCD_CMD_DDRAM_SET | (r_value[row] + col));
}

/* Set LCD controller into a known state and set 4 bit mode */
void lcd_reset (lcd_t *lcd) {
	usleep(45000);
	_pcf8574_put(lcd, LCD_D5 | LCD_D4);

	usleep(5000);
	_pcf8574_put(lcd, LCD_D5 | LCD_D4);

	usleep(1000);
	_pcf8574_put(lcd, LCD_D5 | LCD_D4);

	/* we assume pcf8574 and 4bit mode for now */
	if (lcd->fcn_set & LCD_FCN_8BIT) return;

	_pcf8574_put(lcd, LCD_CMD_FCN_SET | LCD_FCN_4BIT);
}

/* Prints string in actual cursor position */
void lcd_print(lcd_t *lcd, char *instr) {
	int i;
	char *s = instr;

	if (lcd->replace_UTF8_chars) s = _replace_UTF8_chars(instr);

	for (i = 0; i < strlen(s); i++) {
		//printf("Char: %02x\n", s[i]);
		lcd_raw(lcd, LCD_WRITE | LCD_RS, s[i]);
	}

	if (lcd->replace_UTF8_chars) free(s);
}

/* Create characters in the CGRAM table
 * Note that character 0 may be defined, but cannot be used because \x00 is
 * not valid inside a string */
void lcd_create_char(lcd_t *lcd, int n, char *data) {
	if (n < 0 || n > 8) return
	lcd_raw(lcd, LCD_WRITE, LCD_CMD_CGRAM_SET + 8 * n);

	int i;
	for (i = 0; i < 8; i++) {
		lcd_raw(lcd, LCD_WRITE | LCD_RS, data[i]);
	}

	lcd_pos(lcd,0,0);
}


/* Replace non-ascii characters in the string.
 * String must be UTF8.
 * Don't use this if you want actual katakana or
 *  if you have got the european version of HD44780. */
char *_replace_UTF8_chars(char *s) {
	int i = 0; // input counter
	int o = 0; // output counter

	char *t = (char*) malloc(sizeof(char) * strlen(s));

	while (s[i]) {

		t[o] = s[i];

		if (s[i] == 0b11000011) {
			switch (s[i+1]) {
				case 0x81: t[o] = 'A'; break; // Á
				case 0x89: t[o] = 'E'; break; // É
				case 0x8d: t[o] = 'I'; break; // Í
				case 0x93: t[o] = 'O'; break; // Ó
				case 0x9a: t[o] = 'U'; break; // Ú
				case 0x91: t[o] = 0xee; break; // ñ
				case 0xa1: t[o] = 'a'; break; // á
				case 0xa9: t[o] = 'e'; break; // é
				case 0xad: t[o] = 'i'; break; // í
				case 0xb3: t[o] = 'o'; break; // ó
				case 0xba: t[o] = 'u'; break; // ú
				case 0xb1: t[o] = 0xee; break; // ñ
				default:   i--; break; // not interesting
			}
			i++;
		}
		o++;
		i++;
	}
	t[o] = 0;

	return t;
}

void lcd_raw (lcd_t *lcd, int lcd_opts, int data) {
	int upper = (data >> 4) & 0xF;
	int lower = data & 0xF;

	lcd_opts |= lcd->backlight;

	//printf("Data: %02x\n", data);

	_pcf8574_put(lcd, (upper << 4) | lcd_opts);
	_pcf8574_put(lcd, (lower << 4) | lcd_opts);
}

/* Send a nibble and status lines to PCF8574
 * Sets condition err in lcd if some error is detected while sending a command */
void _pcf8574_put (lcd_t *lcd, int lines) {
	//printf("Sending lines: %02x\n", lines);
	i2c_start(lcd->_i2c);

	if (
		(i2c_send_byte(lcd->_i2c, lcd->_addr << 1 | I2C_WRITE) == I2C_ACK) &&
		(i2c_send_byte(lcd->_i2c, lines | LCD_ENABLED) == I2C_ACK) &&
		(i2c_send_byte(lcd->_i2c, lines)               == I2C_ACK)
	) {
		i2c_stop(lcd->_i2c);
		return;
	}

	i2c_stop(lcd->_i2c);
	lcd->err = LCD_ERR_I2C;
}

/* check if PCF8574 driver is ready */
int _pcf8574_check (i2c_t i2c, int addr) {
	i2c_start(i2c);

	int r = i2c_send_byte(
			i2c, 
			addr << 1 | I2C_WRITE);
	
	if (r != I2C_ACK) return 0;
	
	i2c_stop(i2c);
	return 1;
}

