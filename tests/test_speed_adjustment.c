#include "algorithm_challenges/speed_adjustment.h"

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

static void test_known_adjustments(void) {
    const double distances[2U] = {100.0, 100.0};
    const double speeds[2U] = {10.0, 20.0};
    const double target = 100.0 / 15.0 + 100.0 / 25.0;
    const double one_distance[1U] = {100.0};
    const double one_speed[1U] = {10.0};
    double adjustment = 0.0;

    CHECK(close_to(ac_trip_time(distances, speeds, 2U, 5.0), target, 1e-12));
    CHECK(ac_find_speed_adjustment(
        distances,
        speeds,
        2U,
        target,
        &adjustment
    ));
    CHECK(close_to(adjustment, 5.0, 1e-9));

    CHECK(ac_find_speed_adjustment(
        one_distance,
        one_speed,
        1U,
        20.0,
        &adjustment
    ));
    CHECK(close_to(adjustment, -5.0, 1e-9));
}

static void test_invalid_inputs(void) {
    const double distance[1U] = {100.0};
    const double bad_distance[1U] = {-1.0};
    const double speed[1U] = {10.0};
    double adjustment = 0.0;

    CHECK(isinf(ac_trip_time(distance, speed, 1U, -10.0)));
    CHECK(isnan(ac_trip_time(NULL, speed, 1U, 0.0)));
    CHECK(!ac_find_speed_adjustment(NULL, speed, 1U, 10.0, &adjustment));
    CHECK(!ac_find_speed_adjustment(
        bad_distance,
        speed,
        1U,
        10.0,
        &adjustment
    ));
    CHECK(!ac_find_speed_adjustment(distance, speed, 1U, 0.0, &adjustment));
    CHECK(!ac_find_speed_adjustment(distance, speed, 1U, 10.0, NULL));
}

int main(void) {
    test_known_adjustments();
    test_invalid_inputs();

    if (tests_failed != 0U) {
        fprintf(stderr, "%u/%u speed checks failed\n", tests_failed, tests_run);
        return 1;
    }
    printf("speed-adjustment tests: %u checks passed\n", tests_run);
    return 0;
}
