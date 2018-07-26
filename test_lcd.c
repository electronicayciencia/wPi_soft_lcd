/* Program to test LCD
 * Reinoso G.   25/07/2018
 *
 * gcc -lwiringPi -o test_lcd test_lcd.c soft_lcd.c soft_i2c.c -g
 *
 */


#include <stdio.h>
#include "soft_lcd.h"

int main () {
	lcd_t *lcd = lcd_init(25, 24, 0x3f);
	printf("fin\n");
}

