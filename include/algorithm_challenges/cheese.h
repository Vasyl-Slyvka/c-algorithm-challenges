#ifndef ALGORITHM_CHALLENGES_CHEESE_H
#define ALGORITHM_CHALLENGES_CHEESE_H

#include <stdbool.h>
#include <stddef.h>

#define AC_CHEESE_SIDE_MM 100.0

typedef struct {
    double radius_mm;
    double center_z_mm;
} ac_cheese_hole;

double ac_sphere_volume(double radius_mm);

double ac_sphere_volume_below(
    double z_level_mm,
    const ac_cheese_hole *hole
);

double ac_cheese_volume_below(
    double z_level_mm,
    const ac_cheese_hole *holes,
    size_t hole_count
);

bool ac_cheese_slice_thicknesses(
    const ac_cheese_hole *holes,
    size_t hole_count,
    size_t slice_count,
    double *thicknesses_mm
);

#endif
