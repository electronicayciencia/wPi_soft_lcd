INCS=soft_i2c.c soft_lcd.c
HEAD=soft_i2c.h soft_lcd.h
LIBS=-lwiringPi
CFLAGS=-Wall -Wextra -g


all: example_basic example_custom example_intermediate

example_basic: $(INCS) $(HEAD) example_basic.c
	gcc $(LIBS) -o example_basic example_basic.c $(INCS)

example_custom: $(INCS) $(HEAD) example_custom.c
	gcc $(LIBS) -o example_custom example_custom.c $(INCS)

example_intermediate: $(INCS) $(HEAD) example_intermediate.c
	gcc $(LIBS) -o example_intermediate example_intermediate.c $(INCS)

i2cli: $(INCS) $(HEAD) i2cli.c
	gcc $(LIBS) -o i2cli i2cli.c $(INCS)

