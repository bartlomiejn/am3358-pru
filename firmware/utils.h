#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/// Converts uint32_t to a string.
///
/// Replacement for `sprintf` which makes the executable
/// bigger than allowed size and `itoa` which is unimplemented.
void i32_to_string(int32_t n, char *buffer)
{
    int i = 0;
    bool is_neg = n<0;
    uint32_t abs_n = is_neg ? -n : n;
    while(abs_n != 0)
    {
        buffer[i++] = abs_n % 10 + '0';
        abs_n=abs_n / 10;
    }
    if(is_neg)
    {
        buffer[i++] = '-';
    }
    buffer[i] = '\0';
    int t;
    for(t = 0; t < i / 2; t++)
    {
        buffer[t] ^= buffer[i - t - 1];
        buffer[i - t - 1] ^= buffer[t];
        buffer[t] ^= buffer[i - t - 1];
    }
    if(n == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
    }
}

void print_as_ascii(char *string, char *ascii_buf)
{
    char int_buf[16];
    int i, j = 0;
    uint8_t *ptr = (uint8_t*)string;
    while (true)
    {
        if (ptr[i] == 0)
        {
            *(ascii_buf + j) = 48;
            *(ascii_buf + j + 1) = 0;
            break;
        }
        memset(int_buf, 0, 16);
        i32_to_string(ptr[i], int_buf);
        strcat(ascii_buf + j, int_buf);
        i++;
        j += strlen(int_buf) + 1;
        *(ascii_buf + j - 1) = 32;
    }
}
