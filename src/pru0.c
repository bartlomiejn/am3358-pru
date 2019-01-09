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

inline void set_P8_11(bool value)
{
    ui32_set_nth_bit(&__R30, 15, (int)value);
}

inline void set_P8_20(bool value)
{
    ui32_set_nth_bit(&__R30, 13, (int)value);
}

int main(void)
{
    set_P8_11(true);
    set_P8_20(true);
    bool last_p8_15 = get_P8_15();
    bool last_p8_21 = get_P8_21();
    while (true)
    {
        if (get_P8_15() != last_p8_15)
        {
            // TODO: Registering of state changes
        }
    }
    __halt();
    return 0;
}
