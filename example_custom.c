/* Program to test LCD custom characters.
 *
 * Electronicayciencia
 * https://github.com/electronicayciencia/wPi_soft_lcd
 *
 * Compile this way:
 * gcc -lwiringPi -o example_custom example_custom.c soft_lcd.c soft_i2c.c
 *
 * Reinoso G.
 */

#include <stdio.h>
#include <time.h>
#include "soft_lcd.h"

int main () {
	char battery[] = {
		0b01110,
		0b10001,
		0b10001,
		0b10001,
		0b10001,
		0b10001,
		0b10001,
		0b11111
	};


	lcd_t *lcd = lcd_create(23, 24, 0x3f, 2);
	if (lcd == NULL) {
		printf("LCD: Cannot initialize\n");
		return 1;
	}

	int i;
	for (i = 1; i <= 7; i++) {
		printf("Creating character %d...\n",i);
		battery[7-i+1] = 0b11111;
		lcd_create_char(lcd, i, battery);
	}

	lcd_print(lcd, "Energy level:\n");
	lcd_print(lcd, "         \01\02\03\04\05\06\07");

	return 0;
}

