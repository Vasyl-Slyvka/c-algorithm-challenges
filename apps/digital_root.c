#include "algorithm_challenges/digital_root.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static void usage(const char *program) {
    printf(
        "Usage: %s < input\n\n"
        "Reads one arbitrarily long non-negative integer and prints its digital root.\n",
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
    ac_digital_root_state state;
    bool trailing_space = false;
    unsigned int result = 0U;
    int character = 0;
    const int argument_result = parse_arguments(argc, argv);

    if (argument_result > 0) {
        return 0;
    }
    if (argument_result < 0) {
        return 2;
    }

    ac_digital_root_init(&state);
    while ((character = getchar()) != EOF) {
        const unsigned char byte = (unsigned char)character;

        if (isspace(byte) != 0) {
            trailing_space = state.has_digit;
            continue;
        }
        if (trailing_space || !ac_digital_root_push(&state, (char)byte)) {
            fprintf(stderr, "Input must contain exactly one non-negative integer.\n");
            return 2;
        }
    }
    if (!ac_digital_root_finish(&state, &result)) {
        fprintf(stderr, "No integer was provided.\n");
        return 2;
    }
    printf("%u\n", result);
    return 0;
}
