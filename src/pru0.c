#include <stdint.h>
#include "binary.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

inline bool get_P8_15()
{
    return ui32_get_nth_bit(__R31, 15);
}

inline bool get_P8_21()
{
    return ui32_get_nth_bit(__R31, 12);
}

inline void set_P8_11(int value)
{
    ui32_set_nth_bit(&__R30, 15, value);
}

inline void set_P8_20(int value)
{
    ui32_set_nth_bit(&__R30, 13, value);
}

int main(void)
{
    set_P8_11(1);
    set_P8_20(1);
    bool last_p8_15 = get_P8_15();
    bool last_p8_21 = get_P8_21();
    while (true)
    {
        if (get_P8_15() != last_p8_15)
        {

        }
    }
    __halt();
    return 0;
}
