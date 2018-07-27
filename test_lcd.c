/* Program to test LCD
 * Reinoso G.   25/07/2018
 *
 * gcc -lwiringPi -o test_lcd test_lcd.c soft_lcd.c soft_i2c.c -g
 *
 */


#include <stdio.h>
#include <time.h>
#include "soft_lcd.h"

int main () {
	lcd_t *lcd = lcd_create(25, 24, 0x3f);
	lcd_init(lcd);
	lcd_pos(lcd, 0,0);
	lcd_print_str(lcd, "Electronica y");
	lcd_pos(lcd, 1,0);
	lcd_print_str(lcd, "Ciencia");
	
	while (1) {
		char buff[9];

		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

		lcd_pos(lcd, 1,8);
		snprintf(buff, 9, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
		lcd_print_str(lcd, buff);
		
		printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);

		usleep(100000);
	}
	printf("fin\n");
}

