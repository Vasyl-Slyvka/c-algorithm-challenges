#include "algorithm_challenges/bst_shapes.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define AC_NO_CHILD SIZE_MAX

size_t ac_bst_shape_capacity(size_t value_count) {
    if (value_count > (SIZE_MAX - 2U) / 2U) {
        return 0U;
    }
    return value_count * 2U + 2U;
}

static bool ac_bst_build(
    const int *values,
    size_t value_count,
    size_t *left,
    size_t *right
) {
    for (size_t index = 0U; index < value_count; ++index) {
        left[index] = AC_NO_CHILD;
        right[index] = AC_NO_CHILD;
    }

    for (size_t inserted = 1U; inserted < value_count; ++inserted) {
        size_t current = 0U;

        for (;;) {
            size_t *next = values[inserted] < values[current]
                               ? &left[current]
                               : &right[current];

            if (*next == AC_NO_CHILD) {
                *next = inserted;
                break;
            }
            current = *next;
        }
    }
    return true;
}

bool ac_bst_shape_encode(
    const int *values,
    size_t value_count,
    char *shape,
    size_t shape_capacity
) {
    const size_t required = ac_bst_shape_capacity(value_count);
    size_t *left = NULL;
    size_t *right = NULL;
    size_t *stack = NULL;
    size_t stack_size = 0U;
    size_t position = 0U;

    if (shape == NULL || required == 0U || shape_capacity < required ||
        (value_count > 0U && values == NULL)) {
        return false;
    }
    if (value_count == 0U) {
        shape[0U] = '.';
        shape[1U] = '\0';
        return true;
    }
    if (value_count > SIZE_MAX / sizeof(*left) ||
        value_count > SIZE_MAX / sizeof(*stack) - 1U) {
        return false;
    }

    left = malloc(value_count * sizeof(*left));
    right = malloc(value_count * sizeof(*right));
    stack = malloc((value_count + 1U) * sizeof(*stack));
    if (left == NULL || right == NULL || stack == NULL) {
        free(left);
        free(right);
        free(stack);
        return false;
    }

    (void)ac_bst_build(values, value_count, left, right);
    stack[stack_size++] = 0U;
    while (stack_size > 0U) {
        const size_t node = stack[--stack_size];

        if (node == AC_NO_CHILD) {
            shape[position++] = '.';
            continue;
        }
        shape[position++] = 'X';
        stack[stack_size++] = right[node];
        stack[stack_size++] = left[node];
    }
    shape[position] = '\0';

    free(left);
    free(right);
    free(stack);
    return true;
}

bool ac_count_unique_bst_shapes(
    const int *prototypes,
    size_t prototype_count,
    size_t values_per_prototype,
    size_t *unique_count
) {
    const size_t shape_capacity = ac_bst_shape_capacity(values_per_prototype);
    char *shapes = NULL;
    size_t found = 0U;

    if (unique_count == NULL || shape_capacity == 0U ||
        (prototype_count > 0U && prototypes == NULL) ||
        prototype_count > SIZE_MAX / shape_capacity ||
        (prototype_count > 0U &&
         values_per_prototype > SIZE_MAX / prototype_count)) {
        return false;
    }
    if (prototype_count == 0U) {
        *unique_count = 0U;
        return true;
    }
    shapes = malloc(prototype_count * shape_capacity);
    if (shapes == NULL) {
        return false;
    }

    for (size_t prototype = 0U; prototype < prototype_count; ++prototype) {
        char *current = shapes + found * shape_capacity;
        bool is_new = true;

        if (!ac_bst_shape_encode(
                prototypes + prototype * values_per_prototype,
                values_per_prototype,
                current,
                shape_capacity
            )) {
            free(shapes);
            return false;
        }
        for (size_t previous = 0U; previous < found; ++previous) {
            if (strcmp(shapes + previous * shape_capacity, current) == 0) {
                is_new = false;
                break;
            }
        }
        if (is_new) {
            ++found;
        }
    }

    free(shapes);
    *unique_count = found;
    return true;
}
