# C Algorithm Challenges

A compact collection of four algorithmic solvers written in portable C11.
The project grew from my TUKE programming coursework and was restructured into
reusable engines, small command-line frontends, and a deterministic test suite.

| Solver | Main ideas | Complexity |
| --- | --- | --- |
| Cutting Cheese | spherical-cap volume, monotonic volume function, binary search | `O(S × I × H)` |
| BST Shapes | iterative insertion, structural serialization, shape deduplication | `O(P × K² + P² × K)` |
| Digital Root | streaming modulo arithmetic for arbitrarily long input | `O(D)` time, `O(1)` memory |
| Speed Adjustment | monotonic travel-time model and adaptive binary search | `O(N × I)` |

`H` is the number of holes, `S` the number of slices, `P` the number of tree
prototypes, `K` the keys per tree, `D` the input digit count, `N` the route
segment count, and `I` the fixed binary-search iteration count.

## Why these problems

The collection emphasizes different kinds of reasoning instead of four
variations of the same loop:

- translating 3D geometry into a monotonic volume function;
- recognizing that values do not matter when only a BST's structure is compared;
- processing an integer that may be much larger than any built-in numeric type;
- solving an implicit physical equation without a closed-form assumption.

## Build

Requirements:

- CMake 3.20 or newer;
- a C11 compiler (GCC, Clang, or MSVC).

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

On Linux and macOS, `make` is also available.

## Run

```bash
./build/cutting-cheese < input.txt
./build/bst-shapes < input.txt
./build/digital-root < input.txt
./build/speed-adjustment < input.txt
```

Each executable supports `--help` and writes only the requested result to
standard output, so it can be used with redirected files and online judges.

## Solver notes

### Cutting Cheese

Computes horizontal cuts that divide a 100 mm cheese block into equal-volume
slices while accounting for non-overlapping spherical holes. For each proposed
height, the solver subtracts complete spheres or spherical caps from the block
volume, then locates every cut with binary search.

The CLI accepts hole radius and coordinates in micrometres and prints slice
thicknesses in millimetres. The implementation is based on the geometric idea
from the 2015 ICPC World Finals problem *Cutting Cheese*.

### BST Shapes

Builds a binary search tree for every insertion sequence and serializes only
its structure using `X` for a node and `.` for an empty child. Equal strings
therefore identify equal shapes even when their insertion orders differ.
Dynamic storage replaces the original fixed-size buffers.

### Digital Root

Consumes the number digit by digit and keeps only its remainder modulo nine,
plus enough state to distinguish zero from a positive multiple of nine. Input
length is limited by the stream rather than an integer type or buffer size.

### Speed Adjustment

Finds a constant `k` such that

```text
sum(distance[i] / (reported_speed[i] + k)) = target_time
```

The valid lower boundary is derived from the slowest reported speed. The upper
boundary expands adaptively before the final binary search, avoiding a fragile
hard-coded maximum.

## Test

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

For GCC or Clang with AddressSanitizer and UndefinedBehaviorSanitizer:

```bash
make sanitize
```

Tests cover the official hole-free and spherical-hole cheese cases, cap
boundaries, directional BST shapes and duplicates, zero and very long digital
root inputs, positive and negative speed corrections, invalid data, all help
commands, and complete CLI sessions. GitHub Actions runs GCC, Clang, Windows
MSVC, and a separate sanitizer configuration.

## Structure

```text
apps/                          Command-line frontends
include/algorithm_challenges/ Public engine APIs
src/                           Algorithm implementations
tests/                         Unit and CLI integration tests
```

## License and problem attribution

The implementation is available under the MIT License. The license covers the
code in this repository, not the original competitive-programming problem
statements. No assignment statements or private judge data are included.
