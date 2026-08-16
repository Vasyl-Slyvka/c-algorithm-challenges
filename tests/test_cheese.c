#include "algorithm_challenges/cheese.h"

#include <math.h>
#include <stdio.h>

static unsigned int tests_run = 0U;
static unsigned int tests_failed = 0U;

#define CHECK(condition)                                                        \
    do {                                                                        \
        ++tests_run;                                                            \
        if (!(condition)) {                                                     \
            ++tests_failed;                                                     \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); \
        }                                                                       \
    } while (0)

static bool close_to(double actual, double expected, double tolerance) {
    return fabs(actual - expected) <= tolerance;
}

static void test_sphere_caps(void) {
    const ac_cheese_hole hole = {10.0, 20.0};
    const double full = ac_sphere_volume(10.0);

    CHECK(close_to(full, 4188.790204786391, 1e-9));
    CHECK(ac_sphere_volume(-1.0) == 0.0);
    CHECK(ac_sphere_volume_below(10.0, &hole) == 0.0);
    CHECK(close_to(ac_sphere_volume_below(20.0, &hole), full / 2.0, 1e-9));
    CHECK(close_to(ac_sphere_volume_below(30.0, &hole), full, 1e-9));
    CHECK(ac_sphere_volume_below(20.0, NULL) == 0.0);
}

static void test_uniform_slices(void) {
    double thicknesses[4U] = {0.0};
    double total = 0.0;

    CHECK(ac_cheese_slice_thicknesses(NULL, 0U, 4U, thicknesses));
    for (size_t index = 0U; index < 4U; ++index) {
        CHECK(close_to(thicknesses[index], 25.0, 1e-9));
        total += thicknesses[index];
    }
    CHECK(close_to(total, 100.0, 1e-12));
}

static void test_world_finals_sample(void) {
    const ac_cheese_hole holes[2U] = {
        {10.0, 20.0},
        {40.0, 60.0}
    };
    const double expected[5U] = {
        14.611103142,
        16.269801734,
        24.092457788,
        27.002992272,
        18.023645064
    };
    double thicknesses[5U] = {0.0};
    double total = 0.0;

    CHECK(ac_cheese_slice_thicknesses(holes, 2U, 5U, thicknesses));
    for (size_t index = 0U; index < 5U; ++index) {
        CHECK(close_to(thicknesses[index], expected[index], 1e-7));
        total += thicknesses[index];
    }
    CHECK(close_to(total, 100.0, 1e-12));
}

static void test_invalid_inputs(void) {
    const ac_cheese_hole outside = {20.0, 10.0};
    double thickness = 0.0;

    CHECK(!ac_cheese_slice_thicknesses(NULL, 1U, 1U, &thickness));
    CHECK(!ac_cheese_slice_thicknesses(&outside, 1U, 1U, &thickness));
    CHECK(!ac_cheese_slice_thicknesses(NULL, 0U, 0U, &thickness));
    CHECK(!ac_cheese_slice_thicknesses(NULL, 0U, 1U, NULL));
    CHECK(isnan(ac_cheese_volume_below(50.0, &outside, 1U)));
}

int main(void) {
    test_sphere_caps();
    test_uniform_slices();
    test_world_finals_sample();
    test_invalid_inputs();

    if (tests_failed != 0U) {
        fprintf(stderr, "%u/%u cheese checks failed\n", tests_failed, tests_run);
        return 1;
    }
    printf("cheese tests: %u checks passed\n", tests_run);
    return 0;
}
