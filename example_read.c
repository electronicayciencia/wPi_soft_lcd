/* Program to test LCD UTF8 replacement.
 *
 * Electronicayciencia
 * https://github.com/electronicayciencia/wPi_soft_lcd
 *
 * Compile this way:
 * gcc -lwiringPi -o example_utf8 example_utf8.c soft_lcd.c soft_i2c.c
 *
 * Reinoso G.   25/07/2018
 */


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "soft_lcd.h"

int main () {
	int i;
	char buff[30]; // larger than visible
	srand(time(NULL));

	/* Create a LCD given SCL, SDA and I2C address */
	lcd_t *lcd = lcd_create(23, 24, 0x3f, 2);

	if (lcd == NULL) {
		printf("Cannot set-up LCD.\n");
		return 1;
	}
	
	/* Write something random */
	snprintf(buff, 30, "MAX_RAND: %d", RAND_MAX);
	lcd_print(lcd, buff);

	lcd_pos(lcd, 1,0);
	
	snprintf(buff, 30, "Random: %d", rand());
	lcd_print(lcd, buff);

	/* Now read the contents */

	/* Save actual cursor position */
	int old_pos_raw = lcd_read_pos_raw(lcd);
	printf("Cursor was at raw position: %02x\n", old_pos_raw);

	puts("VISIBLE:         ****************");

	/* Read line 1 */
	printf("Line 1 contents: ");
	lcd_pos(lcd, 0,0);
	for (i = 0; i < 32; i++) 
		printf("%c", lcd_read_data(lcd));
	
	/* Read line 2 */
	printf("\nLine 2 contents: ");
	lcd_pos(lcd, 1,0);
	for (i = 0; i < 32; i++) 
		printf("%c", lcd_read_data(lcd));

	puts("");
	puts("VISIBLE:         ****************");
	
	/* Restore old cursor position */
	lcd_pos_raw(lcd, old_pos_raw);
}

