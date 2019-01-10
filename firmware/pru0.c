#include <stdint.h>
#include <stdbool.h>
#include <pru_ctrl.h>
#include "binary.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

inline bool get_P8_15(void)
{
    return ui32_get_nth_bit(__R31, 15);
}

inline bool get_P8_21(void)
{
    return ui32_get_nth_bit(__R31, 12);
}

inline void set_P8_11(bool value)
{
    r32_set_nth_bit(&__R30, 15, (int)value);
}

inline void set_P8_20(bool value)
{
    r32_set_nth_bit(&__R30, 13, (int)value);
}

inline void reset_cycle_count(void)
{
    PRU0_CTRL.CTRL_bit.CYCLE = 0;
}

inline void enable_cycle_counter(void)
{
    PRU0_CTRL.CTRL_bit.EN = 1;
    PRU0_CTRL.CTRL_bit.CTR_EN = 1;
}

int main(void)
{
    set_P8_11(true);
    set_P8_20(true);
    bool last_p8_15 = get_P8_15();
    bool last_p8_21 = get_P8_21();
    reset_cycle_count();
    enable_cycle_counter();
    while (true)
    {
        if (get_P8_15() != last_p8_15)
        {
            // TODO: Outputing cycle count / time
        }
    }
    __halt();
    return 0;
}
