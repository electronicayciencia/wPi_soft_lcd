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
 - **Advanced level**: you know how to use LCD and the different options and commands. You want to have plenty of control over all LCD configurations.

First of all, include header file in your main program:
```c
#include "soft_lcd.h"
```

Since this is not a library, you need to add the source file of LCD and I2C to gcc compile line.
```
gcc -lwiringPi -o example_basic example_basic.c soft_lcd.c soft_i2c.c
```


### Functions (basic level)

#### lcd_t *lcd_create(int scl, int sda, int addr);

Initializes a new I2C bus. The parameters SCL and SDA are SCL and SDA pin numbers. Please use WiringPi numeration. 

After that, tries to communicate with PCF8574 driver and reset the LCD to a known state.

If it goes all right, it returns a new lcd_t structure with the default configuration.

You can have as many LCDs as you need, each connected to different GPIO pins, or in the same if you use different address for the driver.

If you set the wrong pin numbers, I2C bus is busy or not pulled up, your power supply is not ready o LCD driver is defective, or any other error condition, it will return NULL.


## Known bugs and limitations

* Not test for busy flag, just relies in delays.

## See also

[El bus I2C a bajo nivel](http://electronicayciencia.blogspot.com/2017/02/el-bus-i2c-bajo-nivel.html) (Spanish). Extensive description of the bit-banged I2C code.

## Authors

* **Reinoso Guzmán** - *Initial work* - [Electronica y Ciencia](http://electronicayciencia.blogspot.com/)

## License

This is free and unencumbered software released into the public domain. See the [LICENSE](LICENSE) file for details.
