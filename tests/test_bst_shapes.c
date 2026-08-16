#include "algorithm_challenges/bst_shapes.h"

#include <stdio.h>
#include <string.h>

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

static void test_shape_encoding(void) {
    const int balanced[3U] = {2, 1, 3};
    const int balanced_other_order[3U] = {2, 3, 1};
    const int ascending[3U] = {1, 2, 3};
    char first[8U];
    char second[8U];
    char third[8U];
    char empty[2U];

    CHECK(ac_bst_shape_capacity(3U) == 8U);
    CHECK(ac_bst_shape_encode(balanced, 3U, first, sizeof(first)));
    CHECK(ac_bst_shape_encode(
        balanced_other_order,
        3U,
        second,
        sizeof(second)
    ));
    CHECK(ac_bst_shape_encode(ascending, 3U, third, sizeof(third)));
    CHECK(strcmp(first, "XX..X..") == 0);
    CHECK(strcmp(first, second) == 0);
    CHECK(strcmp(first, third) != 0);
    CHECK(ac_bst_shape_encode(NULL, 0U, empty, sizeof(empty)));
    CHECK(strcmp(empty, ".") == 0);
    CHECK(!ac_bst_shape_encode(balanced, 3U, first, 7U));
}

static void test_unique_count(void) {
    const int prototypes[5U][3U] = {
        {2, 1, 3},
        {2, 3, 1},
        {1, 2, 3},
        {3, 2, 1},
        {2, 1, 3}
    };
    const int duplicates[2U][3U] = {
        {2, 2, 1},
        {2, 1, 2}
    };
    size_t unique = 0U;

    CHECK(ac_count_unique_bst_shapes(&prototypes[0U][0U], 5U, 3U, &unique));
    CHECK(unique == 3U);
    CHECK(ac_count_unique_bst_shapes(&duplicates[0U][0U], 2U, 3U, &unique));
    CHECK(unique == 1U);
    CHECK(ac_count_unique_bst_shapes(NULL, 0U, 3U, &unique));
    CHECK(unique == 0U);
    CHECK(!ac_count_unique_bst_shapes(NULL, 1U, 3U, &unique));
    CHECK(!ac_count_unique_bst_shapes(&prototypes[0U][0U], 5U, 3U, NULL));
}

int main(void) {
    test_shape_encoding();
    test_unique_count();

    if (tests_failed != 0U) {
        fprintf(stderr, "%u/%u BST checks failed\n", tests_failed, tests_run);
        return 1;
    }
    printf("BST-shape tests: %u checks passed\n", tests_run);
    return 0;
}
