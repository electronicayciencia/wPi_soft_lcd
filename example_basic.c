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
	lcd_t *lcd = lcd_create(23, 24, 0x3f);

	if (lcd == NULL) {
		printf("Cannot set-up LCD.\n");
		return 1;
	}

	/* Print a string */
	lcd_print(lcd, "Hello World!");

	sleep(5);

	/* Turn off LCD and free the memory */
	lcd_destroy(lcd);
}

