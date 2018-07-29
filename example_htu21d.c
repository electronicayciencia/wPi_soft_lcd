/* Program to test LCD and HTU21D sensor in two different i2c buses.
 *
 * Electronicayciencia
 * https://github.com/electronicayciencia/wPi_soft_lcd
 *
 * Compile this way:
 * gcc -lwiringPi -o example_htu21d example_htu21d.c soft_lcd.c soft_i2c.c
 *
 * Reinoso G.   25/07/2018
 */
 
#include <stdio.h>
#include <wiringPi.h>
#include <stdlib.h>
#include "soft_lcd.h"

#include "soft_i2c.h"

#define HTUADDRESS 0x40

/* This function interacts with HTU sensor */
float read_rht (i2c_t i2c, char cmd) {
	int value;
	int vcheck;

	// Send command
	i2c_start(i2c);

	if (i2c_send_byte(i2c, HTUADDRESS << 1 | I2C_WRITE) != I2C_ACK) {
		fprintf(stderr, "No device found at address %02x.\n", HTUADDRESS);
		return 0;
	}

	i2c_send_byte(i2c, cmd);

	// Read values
	i2c_start(i2c);
	i2c_send_byte(i2c, HTUADDRESS << 1 | I2C_READ);

	value  = (i2c_read_byte(i2c) << 8);
	i2c_send_bit(i2c, I2C_ACK);

	value  = value + i2c_read_byte(i2c) & 0xFFFC; // status bits = 0
	i2c_send_bit(i2c, I2C_ACK);

	vcheck = i2c_read_byte(i2c);
	i2c_send_bit(i2c, I2C_NACK);

	i2c_stop(i2c);

	return (float)value/(float)0x10000;
}


int main (int argc, char **argv)
{
	if (wiringPiSetup () == -1)
		return 1;

	/* Create I2C bus in ports 3 and 6 */
	i2c_t i2c = i2c_init(3, 6);

	/* Create I2C LCD in ports 23 and 24 */
	lcd_t *lcd = lcd_create(23, 24, 0x3f, 2);
	if (lcd == NULL) {
		printf("Cannot set-up LCD.\n");
		return 1;
	}

	/* Print LCD header */
	lcd_print(lcd, "Sensor HTU21D");

	int i = 0;
	while (1) {
		char line2[20];

		/* Read Temperature and Humidity from sensor */
		snprintf (line2, 20, "T:%.1fºC H:%.1f%%",
			-46.85 + 175.72 * read_rht(i2c, 0xe3),
			 -6    + 125    * read_rht(i2c, 0xe5)
		);

		/* Update LCD's second line */
		lcd_pos(lcd, 1,0);
		lcd_print(lcd, line2);

		/* Make sure it is still running */
		i = 1 - i;
		lcd_pos(lcd, 0,15);
		lcd_print(lcd, i ? "+" : "*");

		usleep(1000000);
	}
}

