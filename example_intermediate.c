/* Program to test LCD intermediate usage.
 *
 * Electronicayciencia
 * https://github.com/electronicayciencia/wPi_soft_lcd
 *
 * Compile it this way:
 * gcc -lwiringPi -o example_intermediate example_intermediate.c soft_lcd.c soft_i2c.c
 *
 * Reinoso G.
 */


#include <stdio.h>
#include <time.h>
#include "soft_lcd.h"

int main () {
	lcd_t *lcd = lcd_create(23, 24, 0x3f, 2);
	if (lcd == NULL) {
		printf("LCD not detected.\n");
		return 1;
	}

	lcd_print(lcd, "Electronica y");
	lcd_pos(lcd, 1,0);
	lcd_print(lcd, "Ciencia");

	while (1) {
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

		lcd_pos(lcd, 1,8);
		lcd_printf(lcd, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);

		if (lcd->err) {
			fprintf(stderr, "LCD error detected!\n");

			usleep(100000);
			lcd_reset(lcd);
			lcd_init(lcd);
			lcd_pos(lcd, 0,0);
			lcd_print(lcd, "Error");
			return 1;
		}

		usleep(100000);
	}
	printf("end\n");
}

