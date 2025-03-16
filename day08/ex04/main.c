#include "main.h"

uint8_t			 input[13]	   = {0};
uint8_t			 input_counter = 0;
uint8_t			 input_done	   = 1;
uint8_t			 full_rainbow  = 0;
volatile uint8_t pos		   = 0;

volatile uint8_t leds[3][3] = {0};

uint8_t atob_base(const uint8_t *s)
{
	uint16_t first_digit  = strchr(BASE, s[0]) - BASE;
	uint16_t second_digit = strchr(BASE, s[1]) - BASE;

	return (((first_digit & 0x0F) << 4) | (second_digit & 0x0F));
}

void APA102_start(void)
{
	spi_master_transmit(0);
	spi_master_transmit(0);
	spi_master_transmit(0);
	spi_master_transmit(0);
}

void APA102_transmit_led(uint8_t global, uint8_t r, uint8_t g, uint8_t b)
{
	spi_master_transmit(0xe0 | (global & 0x1f));
	spi_master_transmit(b);
	spi_master_transmit(g);
	spi_master_transmit(r);
}

void APA102_stop(void)
{
	const uint8_t n = (3 / 2) + 1;
	for (uint8_t i = 0; i < n; i++)
	{
		spi_master_transmit(0x00);
	}
}

void led_line_toggle(const uint8_t r, const uint8_t g, const uint8_t b,
					 const uint8_t led)
{
	leds[led][0] = r;
	leds[led][1] = g;
	leds[led][2] = b;
	APA102_start();
	APA102_transmit_led(0x07, leds[0][0], leds[0][1], leds[0][2]);
	APA102_transmit_led(0x07, leds[1][0], leds[1][1], leds[1][2]);
	APA102_transmit_led(0x07, leds[2][0], leds[2][1], leds[2][2]);
	APA102_stop();
}

void print_rainbow(uint8_t r, uint8_t g, uint8_t b)
{
	APA102_start();
	APA102_transmit_led(0x07, r, g, b);
	APA102_transmit_led(0x07, r, g, b);
	APA102_transmit_led(0x07, r, g, b);
	APA102_stop();
}

void wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85)
	{
		print_rainbow(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		print_rainbow(0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		print_rainbow(pos * 3, 255 - pos * 3, 0);
	}
}

void handle_input(void)
{
	if (full_rainbow == 0)
	{
		TIMSK1 &= ~(1 << OCIE1A);
		led_line_toggle(atob_base(input + 1), atob_base(input + 3),
						atob_base(input + 5), input[8] - '6');
	}
	else
	{
		TIMSK1 |= (1 << OCIE1A);
	}
}

uint8_t parse_input()
{
	full_rainbow = 0;
	if (input[0] != '#')
		return (1);
	if (input[1] == 'F')
	{
		if (strcmp((const char *)input + 1, "FULLRAINBOW") == 0)
		{
			full_rainbow = 1;
			memset((char *)leds, 0, 9);
			return (0);
		}
	}
	for (uint8_t i = 1; i < 12; i++)
	{
		uart_tx(input[i]);
		if ((i < 9) && ((input[i] < 'A' || input[i] > 'F') &&
						(input[i] < '0' || input[i] > '9')))
			return (1);
		else if (i >= 9 && input[i] != 0)
			return (1);
	}
	uart_nl();
	if (input[7] != 'D')
		return (1);
	else if (input[8] < '6' || input[8] > '8')
		return (1);
	return (0);
}

void define_timer_1Hz(void)
{
	// Timer1 => mode 4 (CTC), / 256, compare match
	TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
	TCCR1B |= (1 << WGM12) | (1 << CS10);
	TCNT1 = 0;
	OCR1A = 62500;
}

ISR(TIMER1_COMPA_vect)
{
	pos = (pos + 1) % 255;
	wheel(pos);
}

ISR(USART_RX_vect)
{
	uint8_t c = UDR0;
	if (c == '\n' || c == '\r')
	{
		uart_nl();
		input_done = 1;
		if (parse_input() == 0)
			handle_input();
		else
			uart_printstr((uint8_t *)"Format: Invalid\r\n");
		memset(input, 0, 13);
		input_counter = 0;
	}
	else if (c == '\b' || c == 127)
	{
		if (input_counter > 0)
		{
			uart_printstr((uint8_t *)"\b \b");
			input[--input_counter] = '\0';
		}
	}
	else if (input_counter < 12)
	{
		uart_tx(c);
		input[input_counter++] = c;
	}
}

int main(void)
{
	// debug
	DDRB |= (1 << DDB0);

	init_spi_master();
	init_uart();
	define_timer_1Hz();
	SREG |= (1 << SREG_I);
	APA102_start();
	led_line_toggle(0x00, 0x00, 0x00, 0);
	led_line_toggle(0x00, 0x00, 0x00, 1);
	led_line_toggle(0x00, 0x00, 0x00, 2);
	APA102_stop();


	while (1);
}
