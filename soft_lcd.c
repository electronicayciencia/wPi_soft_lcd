#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include "soft_lcd.h"
#include "soft_i2c.h"

/* check if PCF8574 driver is ready */
int _pcf8874_check (lcd_t *lcd) {
	i2c_start(lcd->_i2c);

	int r = i2c_send_byte(
			lcd->_i2c, 
			lcd->_addr << 1 | I2C_WRITE);
	
	if (r != I2C_ACK) return 0;
	
	i2c_stop(lcd->_i2c);
	return 1;
}

lcd_t *lcd_init(int scl, int sda, int addr) {
	lcd_t *lcd = (lcd_t*) malloc(sizeof(lcd_t));

	if (wiringPiSetup () == -1)
	        return NULL;

	lcd->_addr = addr;
	lcd->_i2c  = i2c_init(scl, sda);
	lcd->backlight = 1;

	if ( !_pcf8874_check(lcd) ) return NULL;
	
	lcd_reset(lcd);
	
	lcd_raw(lcd, 
			LCD_WRITE,
			LCD_CMD_DISPLAY_SET | LCD_DISPLAY_ON | LCD_DISPLAY_CURSOR_ON | LCD_DISPLAY_BLINK_OFF);

	lcd_raw(lcd, LCD_WRITE | LCD_RS, 'R');
	lcd_raw(lcd, LCD_WRITE | LCD_RS, 'e');
	lcd_raw(lcd, LCD_WRITE | LCD_RS, 'i');
	lcd_raw(lcd, LCD_WRITE | LCD_RS, 'n');
	lcd_raw(lcd, LCD_WRITE | LCD_RS, 'o');
	lcd_raw(lcd, LCD_WRITE | LCD_RS, 's');
	lcd_raw(lcd, LCD_WRITE | LCD_RS, 'o');
	lcd_raw(lcd, LCD_WRITE | LCD_RS, ' ');
	lcd_raw(lcd, LCD_WRITE | LCD_RS, 'G');
	lcd_raw(lcd, LCD_WRITE | LCD_RS, '.');


	return lcd;
}

/* Reset 4/8bit mode
 * Set 4 bit mode
 * Clear display
 * Cursor home
 * 2 lines
 * LCD ON
 */
void lcd_reset (lcd_t *lcd) {
	/*usleep(45000);
	lcd_raw(lcd, LCD_WRITE, 0b00110000);
	usleep(5000);
	lcd_raw(lcd, LCD_WRITE, 0b00110000);
	usleep(1000);
	lcd_raw(lcd, LCD_WRITE, 0b00110000);*/

	lcd_raw(lcd, 
			LCD_WRITE,
			LCD_CMD_FCN_SET | LCD_FCN_4BIT | LCD_FCN_2LINES | LCD_FCN_5x8);
	
	lcd_raw(lcd, 
			LCD_WRITE,
			LCD_CMD_CURSOR_SET | LCD_CURSOR_MOVE_CUR | LCD_CURSOR_LEFT);

	lcd_clear(lcd);
	lcd_home(lcd);

}


void lcd_home (lcd_t *lcd) {
	lcd_raw(lcd, LCD_WRITE, LCD_CMD_HOME);
}

void lcd_clear (lcd_t *lcd) {
	lcd_raw(lcd, LCD_WRITE, LCD_CMD_CLEAR);
	usleep(2000);
}

void lcd_raw (lcd_t *lcd, int lcd_opts, int data) {
	int upper = (data >> 4) & 0xF;
	int lower = data & 0xF;

	lcd_opts |= (lcd->backlight * LCD_BKLIGHT);

	printf("Data: %02x\n", data);

	_pcf_put(lcd, (upper << 4) | lcd_opts);
	_pcf_put(lcd, (lower << 4) | lcd_opts);
}

int _pcf_put (lcd_t *lcd, int lines) {
	printf("Sending lines: %02x\n", lines);
	i2c_start(lcd->_i2c);
	int r = i2c_send_byte(
			lcd->_i2c, 
			lcd->_addr << 1 | I2C_WRITE);

	if (r != I2C_ACK) return 0;
	
	r = i2c_send_byte(
			lcd->_i2c, 
			lines);

	r = i2c_send_byte(
			lcd->_i2c, 
			lines | LCD_ENABLED);

	r = i2c_send_byte(
			lcd->_i2c, 
			lines);
	
	i2c_stop(lcd->_i2c);

	return 1;
}


