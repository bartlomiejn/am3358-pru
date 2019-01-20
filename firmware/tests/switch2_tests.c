#include "unit_test.h"
#include "doubles/mock_gpi.h"
#include "doubles/mock_gpo.h"
#include "doubles/mock_cycle_counter.h"
#include "software/debouncer.h"
#include "hardware/switch2.h"

#define DEBOUNCE_MS         100
#define CYCLES_PER_MS       200000
#define CYC_RESET_THRESHOLD 4000000000
#define CYCS_50MS           (50 * CYCLES_PER_MS)
#define CYCS_DEBOUNCE       (DEBOUNCE_MS * CYCLES_PER_MS + 1)

static struct switch2 switch2;
static struct debouncer debouncer;
static struct cycle_counter mock_counter;
static struct gpi mock_gpi;
static struct gpo mock_gpo;

static void setup()
{
    mock_cycle_count = 0;
    mock_gpi_value = 0;
    mock_gpi_init(&mock_gpi);
    mock_gpo_init(&mock_gpo);
    mock_cycle_counter_init(&mock_counter, CYCLES_PER_MS, CYC_RESET_THRESHOLD);
    debouncer_init(
        &debouncer,
        DEBOUNCE_MS,
        mock_counter.cycles_per_ms,
        mock_counter.reset_thresh
    );
    switch2_init(
        &switch2,
        &mock_counter,
        &debouncer,
        &mock_gpi,
        &mock_gpo
    );
}

static void teardown() {}

static char *WhenInit_ThenSetGPOTo1(void)
{
    test_assert(spy_gpo_value == 1);
    return 0;
}

static char *WhenInit_ThenSetStateToGPI(void)
{
    test_assert(switch2.state == mock_gpi_value);
    return 0;
}

static char *Given0_WhenGetLastChange_ThenMinus1(void)
{
    switch2.update(&switch2);
    test_assert(switch2.last_on_ms == -1);
    return 0;
}

static char *Given0And50ms_WhenGetLastChange_ThenMinus1(void)
{
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS;
    switch2.update(&switch2);
    test_assert(switch2.last_on_ms == -1);
    return 0;
}

static char *Given0And50msAnd1_WhenGetLastChange_ThenMinus1(void)
{
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS;
    mock_gpi_value = 1;
    switch2.update(&switch2);
    test_assert(switch2.last_on_ms == -1);
    return 0;
}

static char *Given0And50msAnd1AndAfterDebounce_WhenGetLastChange_ThenMinus1(void)
{
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS;
    mock_gpi_value = 1;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE;
    switch2.update(&switch2);
    test_assert(switch2.last_on_ms == -1);
    return 0;
}

static char *Given0And50msAnd1AndDebounceAnd0AndDebounce_WhenGetLastChange_Then100ms(void)
{
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS;
    mock_gpi_value = 1;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE + 1;
    mock_gpi_value = 0;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE * 2 + 1;
    switch2.update(&switch2);
    test_assert(switch2.last_on_ms == 100);
    return 0;
}

static char *Given0And50msAnd1AndDebounceAnd0AndDebounceAnd1AndDebounceAnd50msAnd0AndDebounce_WhenGetLastChange_Then150ms(void)
{
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS;
    mock_gpi_value = 1;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE + 1;
    mock_gpi_value = 0;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE * 2 + 1;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE * 2 + 2;
    mock_gpi_value = 1;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE * 3 + 2;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS * 2 + CYCS_DEBOUNCE * 3 + 2;
    mock_gpi_value = 0;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS * 2 + CYCS_DEBOUNCE * 4 + 2;
    switch2.update(&switch2);
    test_assert(switch2.last_on_ms == 150);
    return 0;
}

static char *Given1_WhenGetLastChange_ThenMinus1(void)
{
    mock_gpi_value = 1;
    switch2.update(&switch2);
    test_assert(switch2.last_on_ms == -1);
    return 0;
}

static char *Given1And50ms_WhenGetLastChange_ThenMinus1(void)
{
    mock_gpi_value = 1;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS;
    switch2.update(&switch2);
    test_assert(switch2.last_on_ms == -1);
    return 0;
}

static char *Given1And50msAnd0_WhenGetLastChange_ThenMinus1(void)
{
    mock_gpi_value = 1;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS;
    mock_gpi_value = 0;
    switch2.update(&switch2);
    test_assert(switch2.last_on_ms == -1);
    return 0;
}

static char *Given1And50msAnd0AndDebounce_WhenGetLastChange_Then50ms(void)
{
    mock_gpi_value = 1;
    switch2.state = 1;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS;
    mock_gpi_value = 0;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS * CYCS_DEBOUNCE;
    switch2.update(&switch2);
    test_assert(switch2.last_on_ms == 50);
    return 0;
}

static char *Given1And50msAnd0AndDebounceAnd1And150msAnd0AndDebounce_WhenGetLastChange_Then150ms(void)
{
    mock_gpi_value = 1;
    switch2.state = 1;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS;
    mock_gpi_value = 0;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE + 1;
    mock_gpi_value = 1;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE * 2 + 1;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS * 2 + CYCS_DEBOUNCE * 2 + 1;
    mock_gpi_value = 0;
    switch2.update(&switch2);
    mock_cycle_count = CYCS_50MS * 2 + CYCS_DEBOUNCE * 3 + 1;
    switch2.update(&switch2);
    test_assert(switch2.last_on_ms == 150);
    return 0;
}

test_case_start(Test_Switch2)
add_test(WhenInit_ThenSetGPOTo1);
add_test(WhenInit_ThenSetStateToGPI);
add_test(Given0_WhenGetLastChange_ThenMinus1);
add_test(Given0And50ms_WhenGetLastChange_ThenMinus1);
add_test(Given0And50msAnd1_WhenGetLastChange_ThenMinus1);
add_test(Given0And50msAnd1AndAfterDebounce_WhenGetLastChange_ThenMinus1);
add_test(Given0And50msAnd1AndDebounceAnd0AndDebounce_WhenGetLastChange_Then100ms);
add_test(Given0And50msAnd1AndDebounceAnd0AndDebounceAnd1AndDebounceAnd50msAnd0AndDebounce_WhenGetLastChange_Then150ms);
add_test(Given1_WhenGetLastChange_ThenMinus1);
add_test(Given1And50ms_WhenGetLastChange_ThenMinus1);
add_test(Given1And50msAnd0_WhenGetLastChange_ThenMinus1);
add_test(Given1And50msAnd0AndDebounce_WhenGetLastChange_Then50ms);
add_test(Given1And50msAnd0AndDebounceAnd1And150msAnd0AndDebounce_WhenGetLastChange_Then150ms);
test_case_end()
