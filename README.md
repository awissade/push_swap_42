*This project has been created as part of the 42 curriculum by **abwissad** and **nychan**.*

# Push_swap

## Description

Push_swap is a sorting challenge written in C. The goal is to sort a stack of integers in ascending order using a second stack as temporary storage and a limited set of operations. The difficulty is not only to sort the values correctly, but to do it with as few moves as possible.

This project is a practical exercise in algorithm design, stack manipulation, and performance optimization. Instead of using a built-in sort, the program rearranges values by moving elements between two stacks according to strict rules.

### Core idea

- Stack A starts with the input values.
- Stack B is used as a helper stack during sorting.
- Only a small set of operations is allowed.
- Each move has a cost, and the goal is to minimize the total number of operations.

### Allowed operations

- `sa`, `sb`, `ss`: swap the top two elements of stack A, stack B, or both.
- `pa`, `pb`: push the top element from one stack to the other.
- `ra`, `rb`, `rr`: rotate a stack upward.
- `rra`, `rrb`, `rrr`: rotate a stack downward.

### Strategy modes

The program can choose different sorting strategies depending on the input:

- `--adaptive` *(default)*: automatically chooses the best strategy based on stack disorder.
- `--simple`: uses a simple algorithm for smaller or more ordered inputs.
- `--medium`: uses a medium-cost algorithm optimized for balanced performance.
- `--complex`: uses a stronger algorithm for larger or more scrambled data.
- `--bench`: prints benchmark information about the selected strategy and total operation count.

## Instructions

### Build the project

Compile the main program with:

```bash
make
# or
make all
```

This creates the `./push_swap` executable.

### Run the program

Use the program with a list of integers:

```bash
./push_swap 4 2 1 3
./push_swap 2 1 3 6 5 8
./push_swap --adaptive --bench 0 -1 2 3 4 100 -100
```

The program validates the input and prints the operation list needed to sort the stack.

### Bonus checker

A checker program is included to validate whether a sequence of moves correctly sorts the stack.

Build the checker with:

```bash
make bonus
```

Then run it like this:

```bash
./checker 3 2 1 0
```

You can also test the generated operations directly:

```bash
ARG="4 67 3 87 23"
./push_swap $ARG | ./checker $ARG
```

### Input rules

- Values must be valid integers.
- Duplicate values are not allowed.
- Numbers must fit inside the signed 32-bit integer range.
- Invalid input ends with an error message and exits cleanly.

## Resources

A few useful references for understanding this challenge and exploring efficient stack-sorting strategies:

- [Push_swap Visualizer](https://awissade.github.io/push_swap_visualizer/)
- Official 42 project concepts and sorting constraints
- Stack-based optimization techniques
- Sliding-window and LIS-related sorting ideas

This project was built as a practical exploration of sorting optimization under strict constraints. The goal was to build a correct and efficient stack-sorting solver while learning how different algorithms behave depending on the structure of the input data.

The repository includes the main sorting program, the checker, and custom parsing and memory-management utilities, making it a complete implementation of the challenge from start to finish.