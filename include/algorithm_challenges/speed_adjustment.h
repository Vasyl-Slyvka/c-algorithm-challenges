#ifndef ALGORITHM_CHALLENGES_SPEED_ADJUSTMENT_H
#define ALGORITHM_CHALLENGES_SPEED_ADJUSTMENT_H

#include <stdbool.h>
#include <stddef.h>

double ac_trip_time(
    const double *distances,
    const double *reported_speeds,
    size_t segment_count,
    double adjustment
);

bool ac_find_speed_adjustment(
    const double *distances,
    const double *reported_speeds,
    size_t segment_count,
    double target_time,
    double *adjustment
);

#endif
