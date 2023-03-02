all:
	avr-gcc -Wall -O1 -s -mmcu=atmega64m1 -o main *.c
	avr-objcopy -j .text -j .data -O ihex main main.hex
clean:
	rm -v main *.hex
