NAME	=	main
CC		=	avr-gcc
COPY	= avr-objcopy

F_CPU	= 16000000UL
UART_BAUDRATE	= 115200

CC_FLAGS	= -Wall -mmcu=atmega328p -DF_CPU=$(F_CPU) -DUART_BAUDRATE=$(UART_BAUDRATE) -O2

SRC		=	main.c

all:	hex flash

hex:	$(SRC)	
	$(CC) $(CC_FLAGS) $< -o $(NAME).bin
	$(COPY) -O ihex $(NAME).bin $(NAME).hex 

flash:
	avrdude $(NAME).hex -p m328p -c arduino -b 115200 -P /dev/ttyUSB0 -U flash:w:$(NAME).hex:i


clean:
	rm main.bin main.hex
