#include "algorithm_challenges/bst_shapes.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AC_MAX_INPUT_ITEMS 1000000U

static void usage(const char *program) {
    printf(
        "Usage: %s < input\n\n"
        "Input: prototype_count values_per_prototype, then each insertion order.\n"
        "Output: number of structurally unique binary-search trees.\n",
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
    size_t prototype_count = 0U;
    size_t values_per_prototype = 0U;
    size_t item_count = 0U;
    size_t unique_count = 0U;
    int *values = NULL;
    const int argument_result = parse_arguments(argc, argv);

    if (argument_result > 0) {
        return 0;
    }
    if (argument_result < 0) {
        return 2;
    }
    if (scanf("%zu %zu", &prototype_count, &values_per_prototype) != 2 ||
        prototype_count == 0U || values_per_prototype == 0U ||
        prototype_count > AC_MAX_INPUT_ITEMS ||
        values_per_prototype > AC_MAX_INPUT_ITEMS ||
        values_per_prototype > SIZE_MAX / prototype_count) {
        fprintf(stderr, "Invalid prototype dimensions.\n");
        return 2;
    }
    item_count = prototype_count * values_per_prototype;
    if (item_count > AC_MAX_INPUT_ITEMS || item_count > SIZE_MAX / sizeof(*values)) {
        fprintf(stderr, "Prototype data is too large.\n");
        return 2;
    }

    values = malloc(item_count * sizeof(*values));
    if (values == NULL) {
        fprintf(stderr, "Could not allocate prototype data.\n");
        return 1;
    }
    for (size_t index = 0U; index < item_count; ++index) {
        if (scanf("%d", &values[index]) != 1) {
            fprintf(stderr, "Invalid prototype value at index %zu.\n", index);
            free(values);
            return 2;
        }
    }

    if (!ac_count_unique_bst_shapes(
            values,
            prototype_count,
            values_per_prototype,
            &unique_count
        )) {
        fprintf(stderr, "Could not classify tree shapes.\n");
        free(values);
        return 1;
    }
    printf("%zu\n", unique_count);
    free(values);
    return 0;
}
