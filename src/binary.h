#include <stdbool.h>

#define BINARY(x) string_to_binary_(#x)

static inline unsigned long long string_to_binary_(const char *s)
{
    unsigned long long i = 0;
    while (*s)
    {
        i <<= 1;
        i += *s++ - '0';
    }
    return i;
}

inline bool ui32_get_nth_bit(uint32_t from, int n)
{
    return (from >> n) & 1;
}

inline void ui32_set_nth_bit(volatile uint32_t *source, int n, int to)
{
    *source = (source & (~(1 << n))) | (to << n);
}
