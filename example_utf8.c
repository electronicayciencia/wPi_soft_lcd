/* Program to test LCD basic usage.
 *
 * Electronicayciencia
 * https://github.com/electronicayciencia/wPi_soft_lcd
 *
 * Compile this way:
 * gcc -lwiringPi -o example_basic example_basic.c soft_lcd.c soft_i2c.c
 *
 * Reinoso G.   25/07/2018
 */


#include <stdio.h>
#include "soft_lcd.h"

int main () {

	/* Create a LCD given SCL, SDA and I2C address */
	lcd_t *lcd = lcd_create(23, 24, 0x3f, 2);

	if (lcd == NULL) {
		printf("Cannot set-up LCD.\n");
		return 1;
	}

	/* Print a string, replacing characters */
	lcd_print(lcd, "España: áéíóú");

	/* Set cursor to second line */
	lcd_pos(lcd, 1,0);
	
	/* Disable characters replacement and print the same string */
	lcd->replace_UTF8_chars = 0;
	lcd_print(lcd, "España: áéíóú");
}

