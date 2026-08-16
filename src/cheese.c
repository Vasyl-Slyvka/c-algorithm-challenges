#include "algorithm_challenges/cheese.h"

#include <math.h>

#define AC_PI 3.14159265358979323846264338327950288
#define AC_CHEESE_SEARCH_ITERATIONS 100U

static bool ac_cheese_hole_is_valid(const ac_cheese_hole *hole) {
    if (hole == NULL || !isfinite(hole->radius_mm) ||
        !isfinite(hole->center_z_mm) || hole->radius_mm <= 0.0) {
        return false;
    }
    return hole->center_z_mm - hole->radius_mm >= 0.0 &&
           hole->center_z_mm + hole->radius_mm <= AC_CHEESE_SIDE_MM;
}

double ac_sphere_volume(double radius_mm) {
    if (!isfinite(radius_mm) || radius_mm <= 0.0) {
        return 0.0;
    }
    return (4.0 / 3.0) * AC_PI * radius_mm * radius_mm * radius_mm;
}

double ac_sphere_volume_below(
    double z_level_mm,
    const ac_cheese_hole *hole
) {
    double cap_height = 0.0;

    if (!ac_cheese_hole_is_valid(hole) || !isfinite(z_level_mm)) {
        return 0.0;
    }
    if (z_level_mm <= hole->center_z_mm - hole->radius_mm) {
        return 0.0;
    }
    if (z_level_mm >= hole->center_z_mm + hole->radius_mm) {
        return ac_sphere_volume(hole->radius_mm);
    }

    cap_height = z_level_mm - (hole->center_z_mm - hole->radius_mm);
    return AC_PI * cap_height * cap_height *
           (hole->radius_mm - cap_height / 3.0);
}

double ac_cheese_volume_below(
    double z_level_mm,
    const ac_cheese_hole *holes,
    size_t hole_count
) {
    double holes_volume = 0.0;
    double clamped_z = z_level_mm;

    if (!isfinite(z_level_mm) || (hole_count > 0U && holes == NULL)) {
        return NAN;
    }
    if (clamped_z < 0.0) {
        clamped_z = 0.0;
    } else if (clamped_z > AC_CHEESE_SIDE_MM) {
        clamped_z = AC_CHEESE_SIDE_MM;
    }

    for (size_t index = 0U; index < hole_count; ++index) {
        if (!ac_cheese_hole_is_valid(&holes[index])) {
            return NAN;
        }
        holes_volume += ac_sphere_volume_below(clamped_z, &holes[index]);
    }

    return AC_CHEESE_SIDE_MM * AC_CHEESE_SIDE_MM * clamped_z -
           holes_volume;
}

bool ac_cheese_slice_thicknesses(
    const ac_cheese_hole *holes,
    size_t hole_count,
    size_t slice_count,
    double *thicknesses_mm
) {
    double previous_z = 0.0;
    double total_volume = 0.0;

    if (slice_count == 0U || thicknesses_mm == NULL ||
        (hole_count > 0U && holes == NULL)) {
        return false;
    }
    total_volume = ac_cheese_volume_below(
        AC_CHEESE_SIDE_MM,
        holes,
        hole_count
    );
    if (!isfinite(total_volume) || total_volume <= 0.0) {
        return false;
    }

    for (size_t slice = 1U; slice < slice_count; ++slice) {
        const double target = total_volume * (double)slice /
                              (double)slice_count;
        double left = previous_z;
        double right = AC_CHEESE_SIDE_MM;

        for (size_t iteration = 0U;
             iteration < AC_CHEESE_SEARCH_ITERATIONS;
             ++iteration) {
            const double middle = (left + right) / 2.0;
            const double current = ac_cheese_volume_below(
                middle,
                holes,
                hole_count
            );

            if (current < target) {
                left = middle;
            } else {
                right = middle;
            }
        }

        thicknesses_mm[slice - 1U] = (left + right) / 2.0 - previous_z;
        previous_z = (left + right) / 2.0;
    }
    thicknesses_mm[slice_count - 1U] = AC_CHEESE_SIDE_MM - previous_z;
    return true;
}
