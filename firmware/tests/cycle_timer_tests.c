#include "cycle_timer_tests.h"
#include "doubles/mock_cycle_counter.h"
#include "software/cycle_timer.h"

#define CYC_PER_MS          200000
#define CYC_RESET_THRESH    4000000000
#define TIM_INTERVAL_MS     1
#define TIM_INTERVAL_CYC    TIM_INTERVAL_MS * CYC_PER_MS

struct cycle_timer timer;
struct cycle_counter mock_counter;

// Setup

test_setup_start(Test_Cycle_Timer)
    mock_cycle_counter_init(&mock_counter, CYC_PER_MS, CYC_RESET_THRESH);
    cycle_timer_init(&timer, &mock_counter, TIM_INTERVAL_MS, CYC_PER_MS);
test_setup_end()

// Single tick

test_function_start(GivenDiffLessThanInterval_WhenTick_ThenFalse)
    mock_cycle_count = TIM_INTERVAL_CYC - 1;
    test_assert(timer.tick(&timer) == false);
test_function_end()

test_function_start(GivenDiffEqualsInterval_WhenTick_ThenFalse)
    mock_cycle_count = TIM_INTERVAL_CYC;
    test_assert(timer.tick(&timer) == false);
test_function_end()

test_function_start(GivenDiffGreaterThanInterval_WhenTick_ThenTrue)
    mock_cycle_count = TIM_INTERVAL_CYC + 1;
    test_assert(timer.tick(&timer) == true);
test_function_end()

// Single tick and cycle reset

test_function_start(GivenDiffLessThanIntervalAndReset_WhenTick_ThenFalse)
    timer.cycle = CYC_RESET_THRESH - 1;
    mock_cycle_count = TIM_INTERVAL_CYC - 2;
    test_assert(timer.tick(&timer) == false);
test_function_end()

test_function_start(GivenDiffEqualsIntervalAndReset_WhenTick_ThenFalse)
    timer.cycle = CYC_RESET_THRESH - 1;
    mock_cycle_count = TIM_INTERVAL_CYC - 1;
    test_assert(timer.tick(&timer) == false);
test_function_end()

test_function_start(GivenDiffGreaterThanIntervalAndReset_WhenTick_ThenTrue)
    timer.cycle = CYC_RESET_THRESH - 1;
    mock_cycle_count = TIM_INTERVAL_CYC;
    test_assert(timer.tick(&timer) == true);
test_function_end()

// Two ticks

test_function_start(GivenTickAndDiffLessThanInterval_WhenTick_ThenFalse)
    mock_cycle_count = TIM_INTERVAL_CYC + 1;
    timer.tick(&timer);
    mock_cycle_count = TIM_INTERVAL_CYC * 2 - 1;
    test_assert(timer.tick(&timer) == false);
test_function_end()

test_function_start(GivenTickAndDiffEqualsInterval_WhenTick_ThenFalse)
    mock_cycle_count = TIM_INTERVAL_CYC + 1;
    timer.tick(&timer);
    mock_cycle_count = TIM_INTERVAL_CYC * 2;
    test_assert(timer.tick(&timer) == false);
test_function_end()

test_function_start(GivenTickAndDiffGreaterThanInterval_WhenTick_ThenTrue)
    mock_cycle_count = TIM_INTERVAL_CYC + 1;
    timer.tick(&timer);
    mock_cycle_count = TIM_INTERVAL_CYC * 2 + 1;
    test_assert(timer.tick(&timer) == true);
test_function_end()

// Two ticks and cycle reset on first

test_function_start(GivenTickAfterResetAndDiffLessThanInterval_WhenTick_ThenFalse)
    timer.cycle = CYC_RESET_THRESH - 1;
    timer.tick(&timer);
    mock_cycle_count = TIM_INTERVAL_CYC;
    timer.tick(&timer);
    mock_cycle_count = TIM_INTERVAL_CYC * 2 - 2;
    test_assert(timer.tick(&timer) == false);
test_function_end()

test_function_start(GivenTickAfterResetAndDiffEqualsInterval_WhenTick_ThenFalse)
    timer.cycle = CYC_RESET_THRESH - 1;
    timer.tick(&timer);
    mock_cycle_count = TIM_INTERVAL_CYC;
    timer.tick(&timer);
    mock_cycle_count = TIM_INTERVAL_CYC * 2 - 1;
    test_assert(timer.tick(&timer) == false);
test_function_end()

test_function_start(GivenTickAfterResetAndDiffGreaterThanInterval_WhenTick_ThenTrue)
    timer.cycle = CYC_RESET_THRESH - 1;
    timer.tick(&timer);
    mock_cycle_count = TIM_INTERVAL_CYC;
    timer.tick(&timer);
    mock_cycle_count = TIM_INTERVAL_CYC * 2;
    test_assert(timer.tick(&timer) == true);
test_function_end()

// Test case

test_case_start(Test_Cycle_Timer)
    add_setup(Test_Cycle_Timer);
    add_test_function(GivenDiffLessThanInterval_WhenTick_ThenFalse);
    add_test_function(GivenDiffEqualsInterval_WhenTick_ThenFalse);
    add_test_function(GivenDiffGreaterThanInterval_WhenTick_ThenTrue);
    
    add_test_function(GivenDiffLessThanIntervalAndReset_WhenTick_ThenFalse);
    add_test_function(GivenDiffEqualsIntervalAndReset_WhenTick_ThenFalse);
    add_test_function(GivenDiffGreaterThanIntervalAndReset_WhenTick_ThenTrue);
    
    add_test_function(GivenTickAndDiffLessThanInterval_WhenTick_ThenFalse);
    add_test_function(GivenTickAndDiffEqualsInterval_WhenTick_ThenFalse);
    add_test_function(GivenTickAndDiffGreaterThanInterval_WhenTick_ThenTrue);
    
    add_test_function(GivenTickAfterResetAndDiffLessThanInterval_WhenTick_ThenFalse);
    add_test_function(GivenTickAfterResetAndDiffEqualsInterval_WhenTick_ThenFalse);
    add_test_function(GivenTickAfterResetAndDiffGreaterThanInterval_WhenTick_ThenTrue);
test_case_end()