#include <stdio.h>
#include "unit_test.h"
#include "cycle_timer_tests.h"
#include "debounced_gpi_tests.h"
#include "switch1_tests.h"
#include "switch2_tests.h"

int tests_ran = 0;
int tests_failed = 0;

int main(void)
{
    Test_Cycle_Timer();
    Test_Debounced_GPI();
    Test_Switch1();
    Test_Switch2();
    if (tests_failed == 0)
    {
        printf("ALL TESTS PASSED\n");
    }
    else
    {
        printf("TESTS FAILED\n");
    }
    printf("Tests ran: %d\nTests failed: %d\n", tests_ran, tests_failed);
    return tests_failed != 0;
}
