# README

Example out:
```
# clang -fsanitize=undefined -Warray-bounds -g lib/util.c main_test.c main.c -o main && valgrind -q --leak-check=yes ./main exec
633080
# clang -fsanitize=undefined -Warray-bounds -g lib/util.c main_test.c main.c -o main && time ./main exec
20048741

real    0m0.143s
user    0m0.138s
sys     0m0.004s
```

Part 6 describes a situation where we can make a boat move by keeping a button
pressed. The longer we hold the button the longer the boat moves, but the boat
can't move while the button is pressed. We need to determine how many
possibilities exist where if we press the button we exceed a specific distance.

Part 6 is an interesting one because it does seem to have a mathematical
solution, a binary search like solution and a brute-force solution. The binary
search solution involves finding the maximum amount of time we can press the
button and exceed the distance, and repeat for the minimum. With these two
values we can calculate the rest. Math solution for next time [here](https://old.reddit.com/r/adventofcode/comments/18cf2s4/2023_day_6_part_2_solved_just_using_math/).



# Build

```
clang -g lib/util.c main_test.c main.c -o main && valgrind -q --leak-check=yes ./main test
```
