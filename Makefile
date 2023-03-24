all:
	avr-gcc -Wall -Wno-main -O1 -s -mmcu=atmega64m1 -o main src/*.c
	avr-objcopy -j .text -j .data -O ihex main main.hex
clean:
	rm -v main *.hex
install:
	avrdude -v -c atmelice_isp -p m64m1 -U flash:r:main.hex:i
