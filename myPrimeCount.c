#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

#define NUM_THREADS 4
#define BATCH_SIZE 1000

static unsigned long int next = 1;

typedef struct {
    uint32_t local_counter;
} ThreadData;

int myRand(void) {
    next = next * 1103515245 + 12345;
    return (int) (next / 65536) % 32768;
}

// Function to perform modular exponentiation (x^y % p) -> https://en.wikipedia.org/wiki/Modular_exponentiation
uint32_t power(uint64_t x, uint32_t y, uint32_t p) {
    uint64_t res = 1;
    x = x % p;                      // Update x if it is more than or equal to p

    while (y > 0) {
        if (y & 1) {
            res = (res * x) % p;    // If y is odd, multiply x with result
        }
        // y must be even now
        y = y >> 1;                 // y = y/2
        x = (x * x) % p;
    }
    return res;
}

// Miller-Rabin primality test -> https://www.geeksforgeeks.org/primality-test-set-3-miller-rabin/
uint8_t isPrime(uint32_t n) {
    if (n <= 1 || n == 4)
        return 0;
    if (n <= 3)
        return 1;

    // Find r such that n = 2^d * r + 1 for some r >= 1
    uint32_t d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
    }
    for (uint32_t i = 0; i < 5; i++) {
        uint32_t a = 2 + myRand() % (n - 4);               // Pick a random number in [2...n-2]
        uint64_t x = power(a, d, n);              // Compute a^d % n

        if (x == 1 || x == n - 1) {
            continue;
        }
        for (uint32_t r = d; r != n - 1; r *= 2) {        // Repeat squaring
            x = (x * x) % n;
            if (x == 1) {
                return 0;
            }
            if (x == n - 1) {
                break;
            }
        }

        if (x != n - 1) {
            return 0;
        }
    }
    return 1;
}

void *processNumbers(void *arg) {
    ThreadData *data = (ThreadData *) arg;
    uint32_t num;
    uint32_t local_counter = 0;

    uint32_t batch[BATCH_SIZE];
    uint32_t batch_idx = 0;

    while (scanf("%d", &num) != EOF) {
        batch[batch_idx++] = num;
        if (batch_idx == BATCH_SIZE) {
            // Process the batch
            for (int i = 0; i < BATCH_SIZE; i++) {
                if (isPrime(batch[i])) {
                    local_counter++;
                }
            }
            batch_idx = 0;
        }
    }

    // Process remaining numbers in the last batch
    for (uint32_t i = 0; i < batch_idx; i++) {
        if (isPrime(batch[i])) {
            local_counter++;
        }
    }

    data->local_counter = local_counter;

    return NULL;
}


int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];

    // Create threads
    for (uint8_t i = 0; i < NUM_THREADS; i++) {
        threadData[i].local_counter = 0;
        pthread_create(&threads[i], NULL, processNumbers, &threadData[i]);
    }

    // Wait for threads to finish
    for (uint8_t i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    uint32_t total_counter = 0;

    // Combine results from all threads
    for (uint8_t i = 0; i < NUM_THREADS; i++) {
        total_counter += threadData[i].local_counter;
    }

    printf("%d total primes.\n", total_counter);

    return 0;
}
