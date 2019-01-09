#include <stdint.h>
#include "binary.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

// P8_15 pr1_pru0_pru_r31_15 Input, Pulldown, Mode 6
// P8_21 pr1_pru1_pru_r31_12 Input, Pulldown, Mode 6
// P8_11 pr1_pru0_pru_r30_15 Output, Pulldown, Mode 6
// P8_20 pr1_pru1_pru_r30_13 Output, Pulldown, Mode 5

inline bool get_P8_15() // P8_21 input
{
    return ui32_get_nth_bit(__R31, 15);
}

inline bool get_P8_21() // P8_15 input
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
