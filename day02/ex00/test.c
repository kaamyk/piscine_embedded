#include <stdio.h>
#include <stdint.h>

int	main (void)
{

	const uint16_t ubrr = (16000000 / (16 * 115200));
	const uint8_t uu = (uint8_t)(ubrr >> 8);
	const uint8_t ii = (uint8_t)(ubrr);
	printf("%hu | %hhu | %hhu\n", ubrr, uu, ii);
}