# Prime Number Counting Task

## [Task Description](HW2.pdf)

In this task, the **goal** is to process an endless data stream of numbers and **count the number of prime numbers
efficiently**. The provided **random number generator - `generator.c`** simulates the endless stream, and the task is to
**beat** the Standard Prime Algorithm **at least x4** (without changing the Prime Algorithm) **or x10** (with changing
the Prime Algorithm) according to the task . The performance of the algorithm is critical, and the solution should be
memory efficient, using **only 2MB of RAM space**.

---

## Implementation Details

### Standard Prime Algorithm - [primeCounter](primeCounter.c)

The standard prime algorithm implemented in the `primeCounter.c` file, uses a simple approach to check if a number is
prime by iterating over potential divisors up to the square root of the number.

### Optimized Prime Algorithm - [myPrimeCount](myPrimeCount.c)

The optimized prime algorithm implemented in the `myPrimeCount.c` file utilizes the
**[Miller-Rabin primality test](https://www.geeksforgeeks.org/primality-test-set-3-miller-rabin/)**, a
probabilistic algorithm that efficiently determines whether a given number is prime. The algorithm is **parallelized**
using **pthreads** to process numbers in **batches**, **reducing** function **calls** to isPrime and **minimizing
mutex**
contention for improved performance.

---

## Benchmarks

### Tested on:

- **Os Name:** Pop!_OS 22.04 LTS
- **CPU:** Intel core i7-9750H CPU - **ONLY 6 Threads over 12** where used for testing
- **Memory:** 32 GiB

| Seed | Size Of Numbers | Time Takes Standard Prime | Time Takes Optimized Prime | Efficiency |
|------|-----------------|---------------------------|----------------------------|------------|
| 10   | 100,000         | 0.435s                    | 0.038s                     | x11.4      |
| 10   | 1,000,000       | 4.201s                    | 0.246s                     | x17.1      |
| 10   | 10,000,000      | 42.053s                   | 2.351s                     | x17.9      |
| 10   | 100,000,000     | 7m2.435s                  | 24.122s                    | x17.5      |
| 10   | 1,000,000,000   | Too Long !                | 4m6.586s                   |            |

---

## Conclusion

The optimized prime algorithm significantly outperforms the standard algorithm across all tested scenarios,
demonstrating the effectiveness of parallelization and algorithmic optimization in improving performance. The optimized
algorithm achieves faster processing times, making it suitable for real-time applications with large data streams.
