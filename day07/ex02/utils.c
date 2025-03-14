#include "utils.h"

uint8_t is_hexa(const char c)
{
	return ((c >= 'a' && c <= 'f') || (c >= '0' && c <= '9'));
}

uint8_t	is_printable(const char c)
{
	return (c > 32 && c < 127);
}

uint16_t atos_base(const uint8_t *s)
{
	uint16_t digits[4] = {0};
	uint8_t	 i		   = 0;

	while (i < 4)
	{
		digits[i] = (strchr(BASE, *s) - BASE);
		s++;
		i++;
	}

	return (((digits[0] & 0x0F) << 12) | ((digits[1] & 0x0F) << 8) |
			((digits[2] & 0x0F) << 4) | (digits[3] & 0x0F));
}

uint8_t atob_base(const char *s)
{
	uint16_t first_digit  = strchr(BASE, s[0]) - BASE;
	uint16_t second_digit = strchr(BASE, s[1]) - BASE;

	return (((first_digit & 0x0F) << 4) | (second_digit & 0x0F));
}

int	ft_strlen(const uint8_t *s)
{
	const uint8_t *tmp = s;
	while (*tmp)
		tmp++;
	return (tmp - s);
}

int ft_strcmp(const uint8_t *s, const uint8_t *s1)
{
	while (*s || *s1)
	{
		if (*s != *s1)
			return (*s - *s1);
		s++;
		s1++;
	}
	return (0);
}
