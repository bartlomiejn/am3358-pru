#include <stdint.h>
#include <stdbool.h>

#define _SWITCH1_ID "switch1"

struct switch1
{
    // TODO: Temporary
    uint32_t _last_change_cyc;
    uint32_t _switch1_start_cyc;
    int32_t _switch1_curr_ms;
    uint32_t _switch1_curr_cyc_plus;
    uint32_t _switch1_curr_cyc_in_ms;
    int32_t _switch1_last_ms;

    bool _state;
    void (*update)(
        struct switch1* self,
        uint32_t curr_cyc,
        void (*pre_change)(void),
        void (*post_change)(void)
    );
};

bool is_switch1_id(char *str);
void switch1_init(struct switch1* self);
