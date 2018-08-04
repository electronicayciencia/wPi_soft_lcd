/* Program to test LCD backlight dimming.
 *
 * Electronicayciencia
 * https://github.com/electronicayciencia/wPi_soft_lcd
 *
 * Compile this way:
 * gcc -lwiringPi -o example_dimming example_dimming.c soft_lcd.c soft_i2c.c
 *
 * Reinoso G.   25/07/2018
 */


#include <stdio.h>
#include <unistd.h>
#include "soft_lcd.h"

int main () {

	/* WARNING: Setting PWM status as a non root user 
	 * may crash some versions of Raspbian. */
	if(geteuid() != 0) {
		puts("This program must be run as root.");
		return 1;
	}

	/* Create a LCD given SCL, SDA and I2C address */
	lcd_t *lcd = lcd_create(23, 24, 0x3f, 2);

	if (lcd == NULL) {
		printf("Cannot set-up LCD.\n");
		return 1;
	}

	lcd_print(lcd, "Dimming example\nValue:");

	while (1) {
		float v = 1;
		
		/* Go Dimmer */
		while (v > 0.001) {
			v *= 0.95; // exponential decay
			lcd_backlight_dim(lcd, v);
			
			lcd_pos(lcd, 1,7);
			lcd_printf(lcd, "%0.3f", v);
			
			usleep(1000);
		}

		/* Go Brighter */
		while (v < 1) {
			v /= 0.95; //exponential growth
			lcd_backlight_dim(lcd, v);
		
			lcd_pos(lcd, 1,7);
			lcd_printf(lcd, "%0.3f", v);
			
			usleep(1000);
		}
	}

	return 0;
}

