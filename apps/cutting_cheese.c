#include "algorithm_challenges/cheese.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AC_INPUT_SIDE_UM INT64_C(100000)
#define AC_MAX_INPUT_ITEMS 1000000U

static void usage(const char *program) {
    printf(
        "Usage: %s < input\n\n"
        "Input: hole_count slice_count, followed by radius/x/y/z in micrometres.\n"
        "Output: slice thicknesses in millimetres.\n",
        program
    );
}

static int parse_arguments(int argc, char **argv) {
    if (argc == 1) {
        return 0;
    }
    if (argc == 2 &&
        (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        usage(argv[0]);
        return 1;
    }
    fprintf(stderr, "Unexpected command-line arguments.\n");
    usage(argv[0]);
    return -1;
}

static bool hole_is_inside_block(
    int64_t radius,
    int64_t x,
    int64_t y,
    int64_t z
) {
    if (radius <= 0 || radius > AC_INPUT_SIDE_UM) {
        return false;
    }
    return x >= radius && x <= AC_INPUT_SIDE_UM - radius &&
           y >= radius && y <= AC_INPUT_SIDE_UM - radius &&
           z >= radius && z <= AC_INPUT_SIDE_UM - radius;
}

int main(int argc, char **argv) {
    size_t hole_count = 0U;
    size_t slice_count = 0U;
    ac_cheese_hole *holes = NULL;
    double *thicknesses = NULL;
    const int argument_result = parse_arguments(argc, argv);

    if (argument_result > 0) {
        return 0;
    }
    if (argument_result < 0) {
        return 2;
    }
    if (scanf("%zu %zu", &hole_count, &slice_count) != 2 ||
        hole_count > AC_MAX_INPUT_ITEMS || slice_count == 0U ||
        slice_count > AC_MAX_INPUT_ITEMS) {
        fprintf(stderr, "Invalid hole or slice count.\n");
        return 2;
    }

    if (hole_count > 0U) {
        holes = malloc(hole_count * sizeof(*holes));
        if (holes == NULL) {
            fprintf(stderr, "Could not allocate hole data.\n");
            return 1;
        }
    }
    thicknesses = malloc(slice_count * sizeof(*thicknesses));
    if (thicknesses == NULL) {
        fprintf(stderr, "Could not allocate slice data.\n");
        free(holes);
        return 1;
    }

    for (size_t index = 0U; index < hole_count; ++index) {
        long long radius = 0LL;
        long long x = 0LL;
        long long y = 0LL;
        long long z = 0LL;

        if (scanf("%lld %lld %lld %lld", &radius, &x, &y, &z) != 4 ||
            !hole_is_inside_block(
                (int64_t)radius,
                (int64_t)x,
                (int64_t)y,
                (int64_t)z
            )) {
            fprintf(stderr, "Invalid spherical hole at index %zu.\n", index);
            free(holes);
            free(thicknesses);
            return 2;
        }
        holes[index].radius_mm = (double)radius / 1000.0;
        holes[index].center_z_mm = (double)z / 1000.0;
    }

    if (!ac_cheese_slice_thicknesses(
            holes,
            hole_count,
            slice_count,
            thicknesses
        )) {
        fprintf(stderr, "Could not calculate slice thicknesses.\n");
        free(holes);
        free(thicknesses);
        return 1;
    }
    for (size_t index = 0U; index < slice_count; ++index) {
        printf("%.9f\n", thicknesses[index]);
    }

    free(holes);
    free(thicknesses);
    return 0;
}
