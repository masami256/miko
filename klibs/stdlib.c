#include <mikoOS/kernel.h>

#define BUF_SIZE (64 + 1)

/**
 * Convert decimal number to string.
 * @param n is what you want to convert to string.
 * @param buf is store converting result.
 */
void itoa(long n, char *buf)
{
	int i, len, sign;
        char tmp[BUF_SIZE] = { 0 };
        i = len = sign = 0;

        if (n < 0) {
		sign = 1;
                n = -n;
        }

        do {
                tmp[len++] = n % 10 + '0';
	}  while ((n /= 10) > 0 && len < BUF_SIZE - 2);

	if (sign)
                tmp[len++] = '-';

	for (i = 0; i < len; i++)
                buf[i] = tmp[len - 1 - i];
        buf[i] = 0x0;
}

/**
 * Convert decimal number to hex string.
 * @param n is what you want to convert to hex string.
 * @param buf is store converting result.
 */
void itox(unsigned long n, char *buf)
{
        int i, len;
        char tmp[BUF_SIZE] = { 0 };
        i = len = 0;
        static const char hex[] = {
                '0', '1', '2', '3', '4', '5',
                '6', '7', '8', '9', 'a', 'b',
                'c', 'd', 'e', 'f' };

        do {
                tmp[len++] = hex[n % 16];
        }  while ((n /= 16) > 0 && len < BUF_SIZE - 1);

        for (i = 0; i < len; i++)
                buf[i] = tmp[len - 1 - i];
        buf[i] = 0x0;
}
