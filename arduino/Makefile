DEVICE = atmega328p
CLOCK = 16000000

AVRDUDE = /usr/bin/avrdude
PORT = /dev/ttyACM0 

BIN = led

OBJ = ${BIN}.o

COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

comp:
	$(COMPILE) -c -o ${OBJ} ${BIN}.c
	$(COMPILE) -o ${BIN}.elf ${OBJ}

hex:
	rm -f ${BIN}.hex
	avr-objcopy -j .text -j .data -O ihex ${BIN}.elf ${BIN}.hex
	avr-size --format=avr --mcu=$(DEVICE) ${BIN}.elf

flash: all
	$(AVRDUDE) -p $(DEVICE) -c arduino -P /dev/ttyACM0 -b115200 -D -Uflash:w:${BIN}.hex:i

all: comp hex

clean:
	rm -f ${OBJ} ${BIN}.elf ${BIN}.hex


	
