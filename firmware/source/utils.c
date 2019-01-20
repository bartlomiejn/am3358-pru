#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"

void ui32_to_str(uint32_t n, char *buffer)
{
    if (n == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
    }
    int i = 0;
    while (n != 0)
    {
        buffer[i++] = n % 10 + '0';
        n = n / 10;
    }
    buffer[i] = '\0';
    int t;
    for (t = 0; t < i / 2; t++)
    {
        buffer[t] ^= buffer[i - t - 1];
        buffer[i - t - 1] ^= buffer[t];
        buffer[t] ^= buffer[i - t - 1];
    }
}

void i32_to_str(int32_t n, char *buffer)
{
    int i = 0;
    bool is_neg = n < 0;
    int32_t abs_n = is_neg ? -n : n;
    while (abs_n != 0)
    {
        buffer[i++] = (uint8_t)(abs_n % 10) + '0';
        abs_n = abs_n / 10;
    }
    if (is_neg)
    {
        buffer[i++] = '-';
    }
    buffer[i] = '\0';
    int t;
    for (t = 0; t < i / 2; t++)
    {
        buffer[t] ^= buffer[i - t - 1];
        buffer[i - t - 1] ^= buffer[t];
        buffer[t] ^= buffer[i - t - 1];
    }
    if (n == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
    }
}

void print_as_ascii(char *string, char *ascii_buf)
{
    char int_buf[16];
    int i = 0, j = 0;
    uint8_t *ptr = (uint8_t*)string;
    while (true)
    {
        if (ptr[i] == 0)
        {
            *(ascii_buf + j) = 48;
            *(ascii_buf + j + 1) = 0;
            break;
        }
        i32_to_str(ptr[i], int_buf);
        strcat(ascii_buf + j, int_buf);
        i++;
        j += strlen(int_buf) + 1;
        *(ascii_buf + j - 1) = 32;
    }
}
