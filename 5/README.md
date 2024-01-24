Part 1
![image](https://github.com/SamJoan/advent-of-code-2023/assets/152786/494b43a2-65cf-4a4d-ae91-c31fbdb7a4cd)

![image](https://github.com/SamJoan/advent-of-code-2023/assets/152786/830fb1cc-8b5f-4702-8167-1ed6b2bb6fd0)

part 2:

<img width="376" alt="image" src="https://github.com/SamJoan/advent-of-code-2023/assets/152786/02970691-db1d-412c-a1ef-99a25faa51f0">

<img width="374" alt="image" src="https://github.com/SamJoan/advent-of-code-2023/assets/152786/dce12415-4c1e-4d1a-9435-1555c0ccd5ec">

The solution above is via pure brute-force of each possible value. Performance can be improved by doing operations directly on intervals. This is the new performance:

```
 clang -g lib/util.c main_test.c main.c -o main && time ./main exec
50716416

real    0m0.003s
user    0m0.000s
sys     0m0.002s
```


# README

```
clang -g lib/util.c main_test.c main.c -o main && valgrind -q --leak-check=yes ./main test
```
