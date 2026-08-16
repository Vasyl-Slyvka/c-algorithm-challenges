#ifndef ALGORITHM_CHALLENGES_BST_SHAPES_H
#define ALGORITHM_CHALLENGES_BST_SHAPES_H

#include <stdbool.h>
#include <stddef.h>

size_t ac_bst_shape_capacity(size_t value_count);

bool ac_bst_shape_encode(
    const int *values,
    size_t value_count,
    char *shape,
    size_t shape_capacity
);

bool ac_count_unique_bst_shapes(
    const int *prototypes,
    size_t prototype_count,
    size_t values_per_prototype,
    size_t *unique_count
);

#endif
