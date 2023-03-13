all:
	avr-gcc -Wall -Wno-main -s -mmcu=atmega64m1 -o main src/*.c
	avr-objcopy -j .text -j .data -O ihex main main.hex
clean:
	rm -v main *.hex
