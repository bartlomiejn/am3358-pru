#include "unit_test.h"
#include "doubles/mock_gpi.h"
#include "doubles/mock_gpo.h"
#include "doubles/mock_cycle_counter.h"
#include "software/debouncer.h"
#include "hardware/switch1.h"
#include "utils.h"

#define DEBOUNCE_MS         100
#define CYCLES_PER_MS       200000
#define CYC_RESET_THRESHOLD 4000000000
#define CYCS_50MS           (50 * CYCLES_PER_MS)
#define CYCS_DEBOUNCE       (DEBOUNCE_MS * CYCLES_PER_MS + 1)

static struct switch1 switch1;
static struct debouncer debouncer;
static struct cycle_counter mock_counter;
static struct gpi mock_gpi;
static struct gpo mock_gpo;

static void setup(void)
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
    switch1_init(
        &switch1,
        &mock_counter,
        &debouncer,
        &mock_gpi,
        &mock_gpo
    );
}

static void teardown(void) {}

static char *WhenInit_ThenSetGPOTo1(void)
{
    test_assert(spy_gpo_value == 1);
    return 0;
}

static char *WhenInit_ThenSetStateToGPI(void)
{
    test_assert(switch1.state == mock_gpi_value);
    return 0;
}

static char *Given0_WhenGetLastChange_ThenMinus1(void)
{
    switch1.update(&switch1);
    test_assert(switch1.last_change_ms == -1);
    return 0;
}

static char *Given50ms_WhenGetLastChange_ThenMinus1(void)
{
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS;
    switch1.update(&switch1);
    test_assert(switch1.last_change_ms == -1);
    return 0;
}

static char *Given50msAndChangeAndBeforeDebounce_WhenGetLastChange_ThenMinus1(void)
{
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS;
    mock_gpi_value = 1;
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE / 2;
    switch1.update(&switch1);
    test_assert(switch1.last_change_ms == -1);
    return 0;
}

static char *Given50msAndChangeAndAfterDebounce_WhenGetLastChange_Then50ms(void)
{
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS;
    mock_gpi_value = 1;
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE;
    switch1.update(&switch1);
    test_assert(switch1.last_change_ms == 50);
    return 0;
}

static char *Given50msAndChangeAndVaryingInputAndAfterDebounce_WhenGetLastChange_Then50ms(void)
{
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS;
    mock_gpi_value = 1;
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS + (CYCS_DEBOUNCE / 4);
    mock_gpi_value = 0;
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS + (CYCS_DEBOUNCE / 2);
    mock_gpi_value = 1;
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE;
    switch1.update(&switch1);
    test_assert(switch1.last_change_ms == 50);
    return 0;
}

static char *Given19975msAndChangeAndCycleOverflowAndAfterDebounce_WhenGetLastChange_Then19975ms(void)
{
    uint32_t cyc_19975ms = CYC_RESET_THRESHOLD - CYCS_DEBOUNCE / 4;
    mock_cycle_count = (CYC_RESET_THRESHOLD - CYCS_DEBOUNCE / 4);
    mock_gpi_value = 1;
    switch1.update(&switch1);
    mock_cycle_count = 0;
    mock_gpi_value = 0;
    switch1.update(&switch1);
    mock_cycle_count = CYCS_DEBOUNCE / 4;
    mock_gpi_value = 1;
    switch1.update(&switch1);
    mock_cycle_count = CYCS_DEBOUNCE * 3 / 4 + 1;
    switch1.update(&switch1);
    test_assert(switch1.last_change_ms == 19975);
    return 0;
}

static char *Given50msAndChangeAnd100msDelayAndChangeAndAfterDebounce_WhenGetLastChange_Then100ms(void)
{
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS;
    mock_gpi_value = 1;
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS + CYCS_DEBOUNCE;
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS * 3;
    mock_gpi_value = 0;
    switch1.update(&switch1);
    mock_cycle_count = CYCS_50MS * 3 + CYCS_DEBOUNCE;
    switch1.update(&switch1);
    test_assert(switch1.last_change_ms == 100);
    return 0;
}

test_case_start(Test_Switch1)
add_test(WhenInit_ThenSetGPOTo1);
add_test(WhenInit_ThenSetStateToGPI);
add_test(Given0_WhenGetLastChange_ThenMinus1);
add_test(Given50ms_WhenGetLastChange_ThenMinus1);
add_test(Given50msAndChangeAndBeforeDebounce_WhenGetLastChange_ThenMinus1);
add_test(Given50msAndChangeAndAfterDebounce_WhenGetLastChange_Then50ms);
add_test(Given50msAndChangeAndVaryingInputAndAfterDebounce_WhenGetLastChange_Then50ms);
add_test(Given50msAndChangeAnd100msDelayAndChangeAndAfterDebounce_WhenGetLastChange_Then100ms);
add_test(Given19975msAndChangeAndCycleOverflowAndAfterDebounce_WhenGetLastChange_Then19975ms);
test_case_end()
