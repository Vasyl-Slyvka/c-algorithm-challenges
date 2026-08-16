.PHONY: all test sanitize clean

CC ?= cc
BUILD_DIR ?= build
CPPFLAGS := -Iinclude
WARNINGS := -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror
CFLAGS ?= -std=c11 -O2
LDLIBS := -lm
SOURCES := src/bst_shapes.c src/cheese.c src/digital_root.c src/speed_adjustment.c

SOLVERS := cutting-cheese bst-shapes digital-root speed-adjustment
TESTS := test_cheese test_bst_shapes test_digital_root test_speed_adjustment

all: $(addprefix $(BUILD_DIR)/,$(SOLVERS))

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/cutting-cheese: $(SOURCES) apps/cutting_cheese.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(WARNINGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/bst-shapes: $(SOURCES) apps/bst_shapes.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(WARNINGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/digital-root: $(SOURCES) apps/digital_root.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(WARNINGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/speed-adjustment: $(SOURCES) apps/speed_adjustment.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(WARNINGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/test_%: $(SOURCES) tests/test_%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(WARNINGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

test: all $(addprefix $(BUILD_DIR)/,$(TESTS))
	./$(BUILD_DIR)/test_cheese
	./$(BUILD_DIR)/test_bst_shapes
	./$(BUILD_DIR)/test_digital_root
	./$(BUILD_DIR)/test_speed_adjustment
	./$(BUILD_DIR)/cutting-cheese --help | grep -q 'Usage:'
	./$(BUILD_DIR)/bst-shapes --help | grep -q 'Usage:'
	./$(BUILD_DIR)/digital-root --help | grep -q 'Usage:'
	./$(BUILD_DIR)/speed-adjustment --help | grep -q 'Usage:'
	./$(BUILD_DIR)/cutting-cheese < tests/fixtures/cheese.in | diff -u tests/fixtures/cheese.out -
	./$(BUILD_DIR)/bst-shapes < tests/fixtures/bst.in | diff -u tests/fixtures/bst.out -
	./$(BUILD_DIR)/digital-root < tests/fixtures/digital_root.in | diff -u tests/fixtures/digital_root.out -
	./$(BUILD_DIR)/speed-adjustment < tests/fixtures/speed.in | diff -u tests/fixtures/speed.out -
	@echo "CLI integration tests: 8 scenarios passed"

sanitize:
	$(MAKE) clean
	ASAN_OPTIONS=detect_leaks=0:halt_on_error=1 \
	UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
	$(MAKE) test \
		BUILD_DIR=build-sanitize \
		CFLAGS='-std=c11 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer' \
		LDFLAGS='-fsanitize=address,undefined'

clean:
	rm -rf build build-sanitize
