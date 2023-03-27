CC = avr-gcc
OUT = main
WNO = -Wno-main -Wno-unused

all:
	$(CC) -Wall $(WNO) -O1 -s -mmcu=atmega64m1 -o $(OUT) src/*.c
	avr-objcopy -j .text -j .data -O ihex $(OUT) $(OUT).hex
clean:
	rm -v $(OUT) *.hex
install:
	avrdude -v -c atmelice_isp -p m64m1 -U flash:w:$(OUT).hex:i
