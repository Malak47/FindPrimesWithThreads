CFLAGS = -O3 -Wall

.PHONY: all
all: generator primeCounter myPrimeCount

generator: generator.c
	gcc -o randomGenerator generator.c

primeCounter: primeCounter.c
	gcc -o primeCounter primeCounter.c

myPrimeCount: myPrimeCount.c
	gcc $(CFLAGS) -o myPrimeCount myPrimeCount.c -lm -pthread

.PHONY: clean

clean:
	-rm randomGenerator primeCounter myPrimeCount 2>/dev/null
