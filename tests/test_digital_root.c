#include "algorithm_challenges/digital_root.h"

#include <stdio.h>

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

static void test_text_inputs(void) {
    unsigned int result = 99U;

    CHECK(ac_digital_root_text("0", &result));
    CHECK(result == 0U);
    CHECK(ac_digital_root_text("000000", &result));
    CHECK(result == 0U);
    CHECK(ac_digital_root_text("45", &result));
    CHECK(result == 9U);
    CHECK(ac_digital_root_text("341", &result));
    CHECK(result == 8U);
    CHECK(ac_digital_root_text("675987890789756545689070986776987", &result));
    CHECK(result == 5U);
    CHECK(!ac_digital_root_text("", &result));
    CHECK(!ac_digital_root_text("12x", &result));
    CHECK(!ac_digital_root_text(NULL, &result));
    CHECK(!ac_digital_root_text("123", NULL));
}

static void test_incremental_state(void) {
    ac_digital_root_state state;
    unsigned int result = 0U;

    ac_digital_root_init(&state);
    CHECK(ac_digital_root_push(&state, '9'));
    CHECK(ac_digital_root_push(&state, '9'));
    CHECK(ac_digital_root_push(&state, '9'));
    CHECK(!ac_digital_root_push(&state, '-'));
    CHECK(ac_digital_root_finish(&state, &result));
    CHECK(result == 9U);
    CHECK(!ac_digital_root_finish(NULL, &result));
}

int main(void) {
    test_text_inputs();
    test_incremental_state();

    if (tests_failed != 0U) {
        fprintf(stderr, "%u/%u digital-root checks failed\n", tests_failed, tests_run);
        return 1;
    }
    printf("digital-root tests: %u checks passed\n", tests_run);
    return 0;
}
