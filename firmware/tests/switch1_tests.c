#include "unit_test.h"
#include "doubles/mock_debounced_gpi.h"
#include "doubles/mock_gpo.h"
#include "hardware/switch1.h"

static struct switch1 switch1;
static struct debounced_gpi mock_gpi;
static struct gpo mock_gpo;

// Auxiliary

static void update_switch_n_times(int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        switch1.update(&switch1);
    }
}

static void gpi_transition_to_unstable_and_update()
{
    mock_gpi.is_stable = true;
    mock_deb_gpi_debounce_is_stable = false;
    switch1.update(&switch1);
}

static void gpi_transition_to_stable_and_update(bool from, bool to)
{
    mock_gpi.state = from;
    mock_deb_gpi_debounce = to;
    mock_gpi.is_stable = false;
    mock_deb_gpi_debounce_is_stable = true;
    switch1.update(&switch1);
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

test_setup_start(Test_Switch1)
    mock_deb_gpi_debounce = 0;
    init_mock_debounced_gpi(&mock_gpi);
    init_mock_gpo(&mock_gpo);
    switch1_init(&switch1, &mock_gpi, &mock_gpo);
test_setup_end()

// Init

test_function_start(WhenInit_ThenSetGPOTo1)
    test_assert(spy_gpo_value == 1);
test_function_end()

test_function_start(WhenInit_ThenSetStateToGPI)
    mock_gpi.state = 1;
    switch1_init(&switch1, &mock_gpi, &mock_gpo);
    test_assert(switch1.state == 1);
test_function_end()

// Last changes - No change

test_function_start(Given0AndUpdate_WhenLastChanges_ThenMinus1)
    switch1.update(&switch1);
    test_assert(switch1.last_time == -1);
test_function_end()

test_function_start(Given0AndUpdateAndChange_WhenLastChanges_ThenMinus1)
    switch1.update(&switch1);
    mock_gpi.is_stable = false;
    switch1.update(&switch1);
    test_assert(switch1.last_time == -1);
test_function_end()

// Last changes - Single change

test_function_start(Given0AndUpdateAndUnstableAndStableWith0_WhenLastChanges_ThenMinus1)
    switch1.update(&switch1);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(0, 0);
    test_assert(switch1.last_time == -1);
test_function_end()

test_function_start(GivenUpdateAndNotStableAndUpdate_WhenLastChanges_ThenMinus1)
    switch1.update(&switch1);
    gpi_transition_to_unstable_and_update();
    test_assert(switch1.last_time == -1);
test_function_end()

test_function_start(GivenUpdateAndUnstableAnd1AndStable_WhenLastChanges_Then1)
    switch1.update(&switch1);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(0, 1);
    test_assert(switch1.last_time == 1);
test_function_end()

test_function_start(Given5UpdatesAndUnstableAndUpdateAnd1AndStable_WhenLastChanges_Then5)
    update_switch_n_times(5);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(0, 1);
    test_assert(switch1.last_time == 5);
test_function_end()

test_function_start(Given5UpdatesAndChangeAnd1AndStable_WhenLastChanges_Then5)
    update_switch_n_times(5);
    gpi_transition_to_unstable_and_update();
    gpi_set_unstable();
    update_switch_n_times(2);
    gpi_transition_to_stable_and_update(0, 1);
    test_assert(switch1.last_time == 5);
test_function_end()

// Last changes - Two changes

test_function_start(Given0AndUpdateAndChangeAnd3UpdatesAndChange_WhenLastChanges_Then5)
    switch1.update(&switch1);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(0, 1);
    gpi_set_stable(1);
    update_switch_n_times(3);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(1, 0);
    test_assert(switch1.last_time == 5);
test_function_end()

test_function_start(Given1AndUpdateAndChangeAnd3UpdatesAndChange_WhenLastChanges_Then5)
    gpi_set_stable(1);
    switch1.update(&switch1);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(1, 0);
    gpi_set_stable(0);
    update_switch_n_times(3);
    gpi_transition_to_unstable_and_update();
    gpi_transition_to_stable_and_update(0, 1);
    test_assert(switch1.last_time == 5);
test_function_end()

test_case_start(Test_Switch1)
    add_setup(Test_Switch1);
    add_test_function(WhenInit_ThenSetGPOTo1);
    add_test_function(WhenInit_ThenSetStateToGPI);
    add_test_function(Given0AndUpdate_WhenLastChanges_ThenMinus1);
    add_test_function(Given0AndUpdateAndChange_WhenLastChanges_ThenMinus1);
    add_test_function(Given0AndUpdateAndUnstableAndStableWith0_WhenLastChanges_ThenMinus1);
    add_test_function(GivenUpdateAndNotStableAndUpdate_WhenLastChanges_ThenMinus1);
    add_test_function(GivenUpdateAndUnstableAnd1AndStable_WhenLastChanges_Then1);
    add_test_function(Given5UpdatesAndUnstableAndUpdateAnd1AndStable_WhenLastChanges_Then5);
    add_test_function(Given5UpdatesAndChangeAnd1AndStable_WhenLastChanges_Then5);
    add_test_function(Given0AndUpdateAndChangeAnd3UpdatesAndChange_WhenLastChanges_Then5);
    add_test_function(Given1AndUpdateAndChangeAnd3UpdatesAndChange_WhenLastChanges_Then5);
test_case_end()
