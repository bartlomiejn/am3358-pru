#include "debounced_gpi_tests.h"
#include "hardware/gpi/debounced_gpi.h"
#include "doubles/mock_gpi.h"

// Setup

struct debounced_gpi deb_gpi;
struct gpi mock_gpi;

test_setup_start(Test_Debounced_GPI)
    mock_gpi_init(&mock_gpi);
test_setup_end()

// Init

test_function_start(WhenInit_ThenSetGPI)
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    test_assert(deb_gpi.gpi == &mock_gpi);
test_function_end()

test_function_start(Given0AndDebounce_WhenIsStable_ThenStable)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    deb_gpi.debounce(&deb_gpi);
    test_assert(deb_gpi.is_stable == true);
test_function_end()

test_function_start(Given1AndDebounce_WhenIsStable_ThenStable)
    mock_gpi_value = 1;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    deb_gpi.debounce(&deb_gpi);
    test_assert(deb_gpi.is_stable == true);
test_function_end()

// Single change

test_function_start(Given0And1AndDebounce_WhenIsStable_ThenUnstable)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    mock_gpi_value = 1;
    deb_gpi.debounce(&deb_gpi);
    test_assert(deb_gpi.is_stable == false);
test_function_end()

test_function_start(Given0And1AndDebounce_WhenState_Then0)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    mock_gpi_value = 1;
    deb_gpi.debounce(&deb_gpi);
    test_assert(deb_gpi.state == 0);
test_function_end()

test_function_start(Given1And0AndDebounce_WhenIsStable_ThenUnstable)
    mock_gpi_value = 1;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    mock_gpi_value = 0;
    deb_gpi.debounce(&deb_gpi);
    test_assert(deb_gpi.is_stable == false);
test_function_end()

test_function_start(Given1And0AndDebounce_WhenState_Then1)
    mock_gpi_value = 1;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    mock_gpi_value = 0;
    deb_gpi.debounce(&deb_gpi);
    test_assert(deb_gpi.state == 1);
test_function_end()

// Multiple changes

void set_alternating_mock_gpi_and_debounce_n_times(bool start, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        mock_gpi_value = (start ? (i + 1) : i) % 2;
        deb_gpi.debounce(&deb_gpi);
    }
}


test_function_start(Given0And5Changes_WhenIsStable_ThenUnstable)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    set_alternating_mock_gpi_and_debounce_n_times(1, 5);
    test_assert(deb_gpi.is_stable == false);
test_function_end()

test_function_start(Given0And5Changes_WhenState_Then0)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    set_alternating_mock_gpi_and_debounce_n_times(1, 5);
    test_assert(deb_gpi.state == 0);
test_function_end()

// Multiple changes and stable

void set_mock_gpi_and_debounce_n_times(bool val, int n)
{
    mock_gpi_value = val;
    int i;
    for (i = 0; i < n; i++)
    {
        deb_gpi.debounce(&deb_gpi);
    }
}

// Multiple changes and stable without change

test_function_start(Given0And5ChangesAnd9times0_WhenIsStable_ThenUnstable)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    set_alternating_mock_gpi_and_debounce_n_times(1, 5);
    set_mock_gpi_and_debounce_n_times(0, 9);
    test_assert(deb_gpi.is_stable == false);
test_function_end()

test_function_start(Given0And5ChangesAnd9times0_WhenState_Then0)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    set_alternating_mock_gpi_and_debounce_n_times(1, 5);
    set_mock_gpi_and_debounce_n_times(0, 9);
    test_assert(deb_gpi.state == 0);
test_function_end()

test_function_start(Given0And5ChangesAnd10times0_WhenIsStable_ThenStable)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    set_alternating_mock_gpi_and_debounce_n_times(1, 5);
    set_mock_gpi_and_debounce_n_times(0, 10);
    test_assert(deb_gpi.is_stable == true);
test_function_end()

test_function_start(Given0And5ChangesAnd10times0_WhenState_Then0)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    set_alternating_mock_gpi_and_debounce_n_times(1, 5);
    set_mock_gpi_and_debounce_n_times(0, 10);
    test_assert(deb_gpi.state == 0);
test_function_end()

// Multiple changes and stable starting from 1

test_function_start(Given0And5ChangesAnd8times1_WhenIsStable_ThenUnstable)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    set_alternating_mock_gpi_and_debounce_n_times(1, 5);
    set_mock_gpi_and_debounce_n_times(1, 8);
    test_assert(deb_gpi.is_stable == false);
test_function_end()

test_function_start(Given0And5ChangesAnd8times1_WhenState_Then0)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    set_alternating_mock_gpi_and_debounce_n_times(1, 5);
    set_mock_gpi_and_debounce_n_times(1, 8);
    test_assert(deb_gpi.state == 0);
test_function_end()

test_function_start(Given0And5ChangesAnd9times1_WhenIsStable_ThenStable)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    set_alternating_mock_gpi_and_debounce_n_times(1, 5);
    set_mock_gpi_and_debounce_n_times(1, 9);
    test_assert(deb_gpi.is_stable == true);
test_function_end()

test_function_start(Given0And5ChangesAnd9times1_WhenState_Then1)
    mock_gpi_value = 0;
    init_debounced_gpi(&deb_gpi, &mock_gpi);
    set_alternating_mock_gpi_and_debounce_n_times(1, 5);
    set_mock_gpi_and_debounce_n_times(1, 9);
    test_assert(deb_gpi.state == 1);
test_function_end()

// Test case

test_case_start(Test_Debounced_GPI)
    add_setup(Test_Debounced_GPI);
    add_test_function(WhenInit_ThenSetGPI);
    add_test_function(Given0AndDebounce_WhenIsStable_ThenStable);
    add_test_function(Given1AndDebounce_WhenIsStable_ThenStable);
    add_test_function(Given0And1AndDebounce_WhenIsStable_ThenUnstable);
    add_test_function(Given0And1AndDebounce_WhenState_Then0);
    add_test_function(Given1And0AndDebounce_WhenIsStable_ThenUnstable);
    add_test_function(Given1And0AndDebounce_WhenState_Then1);
    add_test_function(Given0And5Changes_WhenIsStable_ThenUnstable);
    add_test_function(Given0And5Changes_WhenState_Then0);
    add_test_function(Given0And5ChangesAnd9times0_WhenIsStable_ThenUnstable);
    add_test_function(Given0And5ChangesAnd9times0_WhenState_Then0);
    add_test_function(Given0And5ChangesAnd10times0_WhenIsStable_ThenStable);
    add_test_function(Given0And5ChangesAnd10times0_WhenState_Then0);
    // It is a slight bug that its not symmetric, but it  doesn't matter
    add_test_function(Given0And5ChangesAnd8times1_WhenIsStable_ThenUnstable);
    add_test_function(Given0And5ChangesAnd8times1_WhenState_Then0);
    add_test_function(Given0And5ChangesAnd9times1_WhenIsStable_ThenStable);
    add_test_function(Given0And5ChangesAnd9times1_WhenState_Then1);
test_case_end()
