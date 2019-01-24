#include "unit_test.h"
#include "doubles/mock_debounced_gpi.h"
#include "doubles/mock_gpo.h"
#include "hardware/switch2.h"

static struct switch2 switch2;
static struct debounced_gpi mock_gpi;
static struct gpo mock_gpo;

// Auxiliary

static void update_switch_n_times(int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        switch2.update(&switch2);
    }
}

static void gpi_transition_to_unstable_and_update()
{
    mock_gpi.is_stable = true;
    mock_deb_gpi_debounce_is_stable = false;
    switch2.update(&switch2);
}

static void gpi_transition_to_stable_and_update(int from, int to)
{
    mock_gpi.state = from;
    mock_deb_gpi_debounce = to;
    mock_gpi.is_stable = false;
    mock_deb_gpi_debounce_is_stable = true;
    switch2.update(&switch2);
}

static void gpi_set_unstable()
{
    mock_gpi.is_stable = false;
    mock_deb_gpi_debounce_is_stable = false;
}

static void gpi_set_stable(bool val)
{
    mock_gpi.state = val;
    mock_deb_gpi_debounce = val;
    mock_gpi.is_stable = true;
    mock_deb_gpi_debounce_is_stable = true;
}

// Setup

test_setup_start(Test_Switch2)
    mock_deb_gpi_debounce = 0;
    init_mock_debounced_gpi(&mock_gpi);
    init_mock_gpo(&mock_gpo);
    switch2_init(&switch2, &mock_gpi, &mock_gpo);
test_setup_end()

// Init

test_function_start(WhenInit_ThenSetGPOTo1)
    test_assert(spy_gpo_value == 1);
test_function_end()

test_function_start(WhenInit_ThenSetStateToGPI)
    gpi_set_stable(1);
    switch2_init(&switch2, &mock_gpi, &mock_gpo);
    test_assert(switch2.state == 1);
test_function_end()

// Last on - No change

test_function_start(Given0_WhenLastOn_ThenMinus1)
    gpi_set_stable(0);
    switch2.update(&switch2);
    test_assert(switch2.last_on == -1);
test_function_end()

test_function_start(Given1_WhenLastOn_ThenMinus1)
    gpi_set_stable(1);
    switch2.update(&switch2);
    test_assert(switch2.last_on == -1);
test_function_end()

// Last on - Unstable and single change

test_function_start(Given0AndUpdateAndChangeAnd1_WhenLastOn_ThenMinus1)
    gpi_set_stable(0);
    switch2.update(&switch2);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(0, 1);
    test_assert(switch2.last_on == -1);
test_function_end()

test_function_start(Given1AndUpdateAndChangeAnd0_WhenLastOn_Then1)
    gpi_set_stable(1);
    switch2.update(&switch2);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(1, 0);
    test_assert(switch2.last_on == 1);
test_function_end()

test_function_start(Given0And5UpdatesAndChangeAnd1_WhenLastOn_ThenMinus1)
    gpi_set_stable(0);
    update_switch_n_times(5);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(0, 1);
    test_assert(switch2.last_on == -1);
test_function_end()

test_function_start(Given1And5UpdatesAndChangeAnd0_WhenLastOn_Then5)
    gpi_set_stable(1);
    update_switch_n_times(5);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(1, 0);
    test_assert(switch2.last_on == 5);
test_function_end()

// Last on - Unstable and change twice

test_function_start(Given0AndChangeWith5UpdatesAndChange_WhenLastChanges_Then5)
    gpi_set_stable(0);
    switch2.update(&switch2);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(0, 1);
    gpi_set_stable(1);
    update_switch_n_times(3);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(1, 0);
    test_assert(switch2.last_on == 5);
test_function_end()

test_function_start(Given1AndChangeWith5UpdatesAndChange_WhenLastChanges_Then1)
    gpi_set_stable(1);
    switch2.update(&switch2);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(1, 0);
    gpi_set_stable(0);
    update_switch_n_times(3);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(0, 1);
    test_assert(switch2.last_on == 1);
test_function_end()

// Last on - Unstable and no change

test_function_start(Given0AndUnstableAndStableAnd0_WhenLastOn_ThenMinus1)
    gpi_set_stable(0);
    switch2.update(&switch2);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(0, 0);
    test_assert(switch2.last_on == -1);
test_function_end()

test_function_start(Given0AndUnstableAndStableAnd1_WhenLastOn_ThenMinus1)
    gpi_set_stable(1);
    switch2.update(&switch2);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(1, 1);
    test_assert(switch2.last_on == -1);
test_function_end()

test_case_start(Test_Switch2)
    add_setup(Test_Switch2);
    add_test_function(WhenInit_ThenSetGPOTo1);
    add_test_function(WhenInit_ThenSetStateToGPI);
    add_test_function(Given0_WhenLastOn_ThenMinus1);
    add_test_function(Given1_WhenLastOn_ThenMinus1);
    add_test_function(Given0AndUpdateAndChangeAnd1_WhenLastOn_ThenMinus1);
    add_test_function(Given1AndUpdateAndChangeAnd0_WhenLastOn_Then1);
    add_test_function(Given0And5UpdatesAndChangeAnd1_WhenLastOn_ThenMinus1);
    add_test_function(Given1And5UpdatesAndChangeAnd0_WhenLastOn_Then5);
    add_test_function(Given0AndChangeWith5UpdatesAndChange_WhenLastChanges_Then5);
    add_test_function(Given1AndChangeWith5UpdatesAndChange_WhenLastChanges_Then1);
    add_test_function(Given0AndUnstableAndStableAnd0_WhenLastOn_ThenMinus1);
    add_test_function(Given0AndUnstableAndStableAnd1_WhenLastOn_ThenMinus1);
test_case_end()
