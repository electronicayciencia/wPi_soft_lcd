# Software I2C LCD for Raspberry PI
This is a C library that you can use to connect a PCF8574 based LCD to any of your available GPIO ports. It is feature rich and uses software emulated I2C bus.

## Getting Started

For a quick start guide, please follow these steps:

- Install Gordon's WiringPi library. 
- Download these 4 files and place them into your project directory.
  - [soft_i2c.c](soft_i2c.c)
  - [soft_i2c.h](soft_i2c.h)
  - [soft_lcd.c](soft_lcd.c) 
  - [soft_lcd.h](soft_lcd.h) 
- Include ```soft_lcd.h``` file in your main project.
- Learn commands from the examples.
- And don't forget to include the source files and wiringPi library when compiling:
```
gcc -lwiringPi -o example_basic example_basic.c soft_lcd.c soft_i2c.c
```

If you want to use pins 8 and 9, please notice these are hardware enabled I2C pins. So make sure you unloaded i2c kernel modules before using these routines.

```
rmmod i2c_dev 
rmmod i2c_bcm2708
```

As a rule, always ensure the pins you are using to simulate I2C bus are not taken over by any other software in the background.

### Examples

These are some files that you can use as a quick start guide.

If you want to compile and run them, make sure you modify lcd_create line to match your pin numbers and PCF8574's I2C address.

#### [example_basic.c](example_basic.c)

This is a very basic example. It shows to you how to configure a LCD with the predefined defaults and write some text. That's all.

Just modify the *lcd_create* line to set up your ports. Remember to use WiringPi numbers.

#### [example_intermediate.c](example_intermediate.c)

This is a slightly more complicated example. It shows to you how create a LCD, write test, reposition cursor and check for errors.

It is a clock that displays the string "Electronica y Ciencia" and the current time, updating 10 times a second.

#### [example_custom.c](example_custom.c)

Reading this example you will learn how to create and show custom characters.



## Usage

There are several levels to use this library:

 - **Basic level**: just power on the LCD and display some messages.
 - **Intermediate level**: the above plus some convenient functions like cursor position, clearing screen, cursor types, backlight control or custom characters.
 - **Advanced level**: you know how to use LCDs and the different options and commands they have. You want to have plenty of control over all LCD configurations.

First of all, include header file in your main program:
```c
#include "soft_lcd.h"
```

Since this is not a library, you need to add the source file of LCD and I2C to gcc compile line.
```
gcc -lwiringPi -o example_basic example_basic.c soft_lcd.c soft_i2c.c
```


### Functions (level: basic)

#### lcd_t *lcd_create(int scl, int sda, int addr);

Initializes a new I2C bus. The parameters SCL and SDA are SCL and SDA pin numbers. Please use WiringPi numeration. 

After that, tries to communicate with PCF8574 driver and reset the LCD to a known state.

If it goes all right, it returns a new lcd_t structure with the default configuration.

You can have as many LCDs as you need, each connected to different GPIO pins, or in the same if you use different address for the driver.

If you set the wrong pin numbers, I2C bus is busy or not pulled up, your power supply is not ready o LCD driver is defective, or any other error condition, it will return NULL.

```c
/* Create a LCD given SCL, SDA and I2C address */
lcd_t *lcd = lcd_create(23, 24, 0x3f);

if (lcd == NULL) {
	printf("Cannot set-up LCD.\n");
	return 1;
}
```

#### void lcd_print(lcd_t *lcd, char *string);
Display the text given by *string* parameter in the LCD created before at the current cursor position. It overwrites the existing text.

```c
/* Print a string */
lcd_print(lcd, "Hello World!");
```

#### void lcd_pos(lcd_t *lcd, int row, int col);
Move the cursor to the position given by *row* and *col* parameters. These start in 0; so the first row is 0, the second is 1, and so on. The same for columns. 

The home position is 0,0.

```c
/* Move the cursor to the start of the second line */
lcd_pos(lcd, 1, 0);
```

#### void lcd_destroy(lcd_t *lcd)
Turn off the display, the backlight and free the memory. It is optional to call this function. If you don't call it at the end of your program, the LCD just shows the last message until you disconnect the power supply.

```c
/* Turn off LCD and free the memory */
lcd_destroy(lcd);
```

### Functions (level: intermediate)

#### void lcd_clear(lcd_t *lcd);
This function clears all text, return the display to the home position if shifted and moves cursor to the top left corner.

```c
/* Clear all contents */
lcd_clear(lcd);
```

#### void lcd_home(lcd_t *lcd);
This function return the display to the home position if shifted and moves cursor to the top left corner.

It is different from *lcd_pos* because this only moves the cursor, but does not affect shifting.

```c
/* Return cursor and display to top-left */
lcd_home(lcd);
```

#### void lcd_on(lcd_t *lcd);
Turns LCD ON (default). When the LCD is on, the display data can be displayed instantly.

#### void lcd_off(lcd_t *lcd);
When the LCD is off, the display data remains in RAM, and can be displayed instantly by setting D to 1.

#### void lcd_backlight_on(lcd_t *lcd);
Turns backlight ON (default).

#### void lcd_backlight_off(lcd_t *lcd);
Turns backlight OFF.

#### void lcd_cursor_on(lcd_t *lcd);
Makes cursor visible.

#### void lcd_cursor_off(lcd_t *lcd);
Makes cursor not visible (default).

#### void lcd_blink_on(lcd_t *lcd);
Enable cursor blinking.

#### void lcd_blink_off(lcd_t *lcd);
Disable cursor blinking (default).

#### void lcd_create_char(lcd_t *lcd, int n, char *data);
Stores a custom 5x8 character in the position *n* of the Character Generator RAM. You can use these characters later with codes 0x00 to 0x07.

Please note that character 0x00 is not allowed inside a string, making this character not usable with basic and intermediate functions.

To make a custom character you need to create a binary array.
```c
char mychar[] = {
	0b01110,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b11111
};

lcd_create_char(lcd, 2, mychar);

lcd_print(lcd, "My char: \02");
```

### Functions (level: advanced)

#### void lcd_init(lcd_t *lcd);
#### void lcd_reconfig_fcn(lcd_t *lcd);
#### void lcd_reconfig_cursor(lcd_t *lcd);
#### void lcd_reconfig_display(lcd_t *lcd);
#### void lcd_reconfig_entrymode(lcd_t *lcd);
#### void lcd_reconfig(lcd_t *lcd);

#### void lcd_reset (lcd_t *lcd);
#### void lcd_raw (lcd_t *lcd, int lcd_opts, int data);


## Known bugs and limitations

* Not testing for busy flag, just relies in delays.

## See also

[El bus I2C a bajo nivel](http://electronicayciencia.blogspot.com/2017/02/el-bus-i2c-bajo-nivel.html) (Spanish). Extensive description of the bit-banged I2C code.

## Authors

* **Reinoso Guzmán** - *Initial work* - [Electronica y Ciencia](http://electronicayciencia.blogspot.com/)

## License

This is free and unencumbered software released into the public domain. See the [LICENSE](LICENSE) file for details.
