NAME	=	main
CC		=	avr-gcc
COPY	=	avr-objcopy

F_CPU	= 16000000UL
UART_BAUDRATE	= 115200

CC_FLAGS	= -Wall -mmcu=atmega328p -DF_CPU=$(F_CPU) -DUART_BAUDRATE=$(UART_BAUDRATE) -O2

SRC		=	main.c	uart.c

OBJDIR	=	.obj
OBJ		=	$(patsubst %.c, $(OBJDIR)/%.o, $(SRC))

all:	hex flash

$(OBJDIR):
	mkdir -v $(OBJDIR)
	
$(OBJDIR)/%.o:	%.c
	avr-gcc $(CC_FLAGS) -c $< -o $@
	
hex:	$(OBJDIR) $(OBJ)
	$(CC) $(CC_FLAGS) $(OBJ) -o $(NAME).bin
	$(COPY) -O ihex $(NAME).bin $(NAME).hex 

flash:
	avrdude $(NAME).hex -p m328p -c arduino -b $(UART_BAUDRATE) -P /dev/ttyUSB0 -U flash:w:$(NAME).hex:i

clean:
	rm main.bin main.hex
	rm -r .obj
