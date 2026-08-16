#include "algorithm_challenges/speed_adjustment.h"

#include <float.h>
#include <math.h>

#define AC_SPEED_SEARCH_ITERATIONS 200U
#define AC_SPEED_BOUND_ITERATIONS 1024U

static bool ac_speed_input_is_valid(
    const double *distances,
    const double *reported_speeds,
    size_t segment_count
) {
    if (distances == NULL || reported_speeds == NULL || segment_count == 0U) {
        return false;
    }
    for (size_t index = 0U; index < segment_count; ++index) {
        if (!isfinite(distances[index]) || distances[index] <= 0.0 ||
            !isfinite(reported_speeds[index])) {
            return false;
        }
    }
    return true;
}

double ac_trip_time(
    const double *distances,
    const double *reported_speeds,
    size_t segment_count,
    double adjustment
) {
    double total = 0.0;

    if (!ac_speed_input_is_valid(distances, reported_speeds, segment_count) ||
        !isfinite(adjustment)) {
        return NAN;
    }
    for (size_t index = 0U; index < segment_count; ++index) {
        const double actual_speed = reported_speeds[index] + adjustment;

        if (actual_speed <= 0.0) {
            return INFINITY;
        }
        total += distances[index] / actual_speed;
    }
    return total;
}

bool ac_find_speed_adjustment(
    const double *distances,
    const double *reported_speeds,
    size_t segment_count,
    double target_time,
    double *adjustment
) {
    double minimum_speed = 0.0;
    double left = 0.0;
    double right = 1.0;

    if (adjustment == NULL || !isfinite(target_time) || target_time <= 0.0 ||
        !ac_speed_input_is_valid(distances, reported_speeds, segment_count)) {
        return false;
    }

    minimum_speed = reported_speeds[0U];
    for (size_t index = 1U; index < segment_count; ++index) {
        if (reported_speeds[index] < minimum_speed) {
            minimum_speed = reported_speeds[index];
        }
    }
    left = nextafter(-minimum_speed, INFINITY);
    if (!isfinite(left)) {
        return false;
    }
    if (right <= left) {
        right = left + 1.0;
    }

    for (size_t iteration = 0U;
         iteration < AC_SPEED_BOUND_ITERATIONS &&
         ac_trip_time(distances, reported_speeds, segment_count, right) >
             target_time;
         ++iteration) {
        if (right > (DBL_MAX - 1.0) / 2.0) {
            return false;
        }
        right = right * 2.0 + 1.0;
    }
    if (!isfinite(right) ||
        ac_trip_time(distances, reported_speeds, segment_count, right) >
            target_time) {
        return false;
    }

    for (size_t iteration = 0U;
         iteration < AC_SPEED_SEARCH_ITERATIONS;
         ++iteration) {
        const double middle = (left + right) / 2.0;
        const double current_time = ac_trip_time(
            distances,
            reported_speeds,
            segment_count,
            middle
        );

        if (current_time > target_time) {
            left = middle;
        } else {
            right = middle;
        }
    }
    *adjustment = (left + right) / 2.0;
    return true;
}
