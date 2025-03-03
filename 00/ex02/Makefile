NAME	=	main
CC		=	avr-gcc
COPY	= avr-objcopy

CC_FLAGS	= -Wall -Wcpp -mmcu=atmega328p -DF_CPU=16000000UL

SRC		=	main.c

all:	hex flash

hex:	$(SRC)	
	$(CC) $(CC_FLAGS) $< -o $(NAME).bin
	$(COPY) -O ihex $(NAME).bin $(NAME).hex 

flash:
	avrdude $(NAME).hex -p m328p -c arduino -b 115200 -P /dev/ttyUSB0 -U flash:w:$(NAME).hex:i


clean:
	rm main.bin main.hex
