#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "spi.h"

#define BASE "0123456789abcdef"

uint8_t led_selector   = 0;
uint8_t color_selector = 0;

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

void led_line_toggle(void)
{
	APA102_start();
	APA102_transmit_led(31, leds[0][0], leds[0][1], leds[0][2]);
	APA102_transmit_led(31, leds[1][0], leds[1][1], leds[1][2]);
	APA102_transmit_led(31, leds[2][0], leds[2][1], leds[2][2]);
	APA102_stop();
}

void setup_pv1(void)
{
	//	Select the ADC0
	// ADMUX &= 0b0000;
	//	Select the voltage | Left adjust result
	ADMUX |= (1 << REFS0) | (1 << ADLAR);

	//	Enable ADC | divider = 128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void debounce(uint8_t button)
{
	if (!(PIND & (1 << button)))
	{
		_delay_ms(50);
		while (!(PIND & (1 << button))) _delay_ms(20);
	}
}

int main(void)
{
	uint8_t value = 0;

	setup_pv1();
	init_spi_master();
	APA102_start();
	led_line_toggle();
	APA102_stop();
	while (1)
	{
		//	After each conversion the flag is set to 0
		//	Set the ADSC bit in ADCSRA
		ADCSRA |= (1 << ADSC);
		//	Wait for the conversion to be done
		while (ADCSRA & (1 << ADSC));
		value = ADCH;
		if (~PIND & (1 << PD2))
		{
			debounce(PD2);
			color_selector = (color_selector + 1) % 3;
		}
		else if (~PIND & (1 << PD4))
		{
			debounce(PD4);
			led_selector = (led_selector + 1) % 3;
			color_selector = 0;
		}
		leds[led_selector][color_selector] = value;
		led_line_toggle();
	}
}
