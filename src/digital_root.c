#include "algorithm_challenges/digital_root.h"

#include <stddef.h>

void ac_digital_root_init(ac_digital_root_state *state) {
    if (state != NULL) {
        state->remainder = 0U;
        state->has_digit = false;
        state->has_nonzero_digit = false;
    }
}

bool ac_digital_root_push(ac_digital_root_state *state, char digit) {
    unsigned int value = 0U;

    if (state == NULL || digit < '0' || digit > '9') {
        return false;
    }
    value = (unsigned int)(digit - '0');
    state->remainder = (state->remainder + value) % 9U;
    state->has_digit = true;
    state->has_nonzero_digit = state->has_nonzero_digit || value != 0U;
    return true;
}

bool ac_digital_root_finish(
    const ac_digital_root_state *state,
    unsigned int *result
) {
    if (state == NULL || result == NULL || !state->has_digit) {
        return false;
    }
    if (!state->has_nonzero_digit) {
        *result = 0U;
    } else if (state->remainder == 0U) {
        *result = 9U;
    } else {
        *result = state->remainder;
    }
    return true;
}

bool ac_digital_root_text(const char *digits, unsigned int *result) {
    ac_digital_root_state state;

    if (digits == NULL) {
        return false;
    }
    ac_digital_root_init(&state);
    for (size_t index = 0U; digits[index] != '\0'; ++index) {
        if (!ac_digital_root_push(&state, digits[index])) {
            return false;
        }
    }
    return ac_digital_root_finish(&state, result);
}
