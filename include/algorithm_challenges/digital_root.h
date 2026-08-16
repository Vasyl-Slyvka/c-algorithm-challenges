#ifndef ALGORITHM_CHALLENGES_DIGITAL_ROOT_H
#define ALGORITHM_CHALLENGES_DIGITAL_ROOT_H

#include <stdbool.h>

typedef struct {
    unsigned int remainder;
    bool has_digit;
    bool has_nonzero_digit;
} ac_digital_root_state;

void ac_digital_root_init(ac_digital_root_state *state);

bool ac_digital_root_push(ac_digital_root_state *state, char digit);

bool ac_digital_root_finish(
    const ac_digital_root_state *state,
    unsigned int *result
);

bool ac_digital_root_text(const char *digits, unsigned int *result);

#endif
