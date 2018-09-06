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
#include <unistd.h>
#include "soft_lcd.h"

int main () {

	/* Create a LCD given SCL, SDA and I2C address, 4 lines */
	/* PCF8574 has default address 0x27 */
	lcd_t *lcd = lcd_create(23, 24, 0x27, 4);

	if (lcd == NULL) {
		printf("Cannot set-up LCD.\n");
		return 1;
	}

	/* Print a string in each of 4 lines */
	lcd_print(lcd, "This is line one.\n");
	lcd_print(lcd, "This is line two.\n");
	lcd_print(lcd, "This is line three.\n");
	lcd_print(lcd, "This is line four.\n");

	return 0;
}

