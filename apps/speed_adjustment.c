#include "algorithm_challenges/speed_adjustment.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AC_MAX_SEGMENTS 1000000U

static void usage(const char *program) {
    printf(
        "Usage: %s < input\n\n"
        "Input: segment_count target_time, followed by distance/reported_speed pairs.\n"
        "Output: the constant speedometer adjustment.\n",
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

int main(int argc, char **argv) {
    size_t segment_count = 0U;
    double target_time = 0.0;
    double adjustment = 0.0;
    double *distances = NULL;
    double *speeds = NULL;
    const int argument_result = parse_arguments(argc, argv);

    if (argument_result > 0) {
        return 0;
    }
    if (argument_result < 0) {
        return 2;
    }
    if (scanf("%zu %lf", &segment_count, &target_time) != 2 ||
        segment_count == 0U || segment_count > AC_MAX_SEGMENTS ||
        segment_count > SIZE_MAX / sizeof(*distances)) {
        fprintf(stderr, "Invalid segment count or target time.\n");
        return 2;
    }

    distances = malloc(segment_count * sizeof(*distances));
    speeds = malloc(segment_count * sizeof(*speeds));
    if (distances == NULL || speeds == NULL) {
        fprintf(stderr, "Could not allocate segment data.\n");
        free(distances);
        free(speeds);
        return 1;
    }
    for (size_t index = 0U; index < segment_count; ++index) {
        if (scanf("%lf %lf", &distances[index], &speeds[index]) != 2) {
            fprintf(stderr, "Invalid segment at index %zu.\n", index);
            free(distances);
            free(speeds);
            return 2;
        }
    }

    if (!ac_find_speed_adjustment(
            distances,
            speeds,
            segment_count,
            target_time,
            &adjustment
        )) {
        fprintf(stderr, "Could not solve for a valid speed adjustment.\n");
        free(distances);
        free(speeds);
        return 2;
    }
    printf("%.9f\n", adjustment);
    free(distances);
    free(speeds);
    return 0;
}
