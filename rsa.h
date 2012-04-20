#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Generate a prime number
long getPrime() {
  long value = 0;
  while(1) {
    value = rand() % 10000;
    int check = 0;
    int i;

    for(i = 2; i < value; i++) {
      if(value % i == 0) {
	check = 1;
      }
    }
    if(check == 0) {
      break;
    }
  }
  return value;
}

long getM(long a, long b) {
  return (a - 1) * (b - 1);
}

// Compute the GCD of two numbers a and b
long gcd(long a, long b) {
  long remainder = 0;

  if((a != 0) && (b != 0)) {
    if(a < b) {
      long temp = a;
      a = b;
      b = temp;
    }

    while(a % b > 0) {
      remainder = a % b;
      a = b;
      b = remainder;
    }
  } else {
    return 0;
  }

  return b;
}

// Computes Math.mod(Math.pow(a, b), c), for large values of a, b, c
long modulo(long a, long b, long c) {
  long solution = 1;
  long length = 0;

  while(b > 0) {
    long index = b % 2;
    if(index == 1) {
      b = (b - 1) / 2;
    } else {
      b = b / 2;
    }

    if(index == 1) {
      long term = (long)a;
      long counter = 1;

      while(counter <= length) {
	if(length != 0) {
	  term = (term * term) % c;
	}
	counter += 1;
      }
      solution = (solution * term) % c;
    }
    length++;
  }
  if(solution < 0) {
    solution = solution + c;
  }
  return solution;
}

// Using a random number generator, pick random long that is coprime to x
long coprime(long x) {
  long value = rand() % x;
  
  while((gcd(value, x) != 1) || (value < 0)) {
    value++;
  }

  return value;
}

// Given an integer representing an ASCII value, encrypt it via RSA
int endecrypt(int msg, long key, long c) {
  return modulo(msg, key, c);
}

// Compute the modular inverse base^-1 % m
long mod_inverse(long base, long m) {
  long mod = m;
  // Check to make sure base and m are coprime
  if(gcd(base, m) != 1) {
    return 0;
  } 
  long preA = 1, preB = 0;
  long quotient[1000];
  long remainder = 0, counter = 0, A = 0, B = 0;

  while(base % m != 0) {
    remainder = base % m;
    base = m;
    m = remainder;
    quotient[counter] = base / m;
    counter++;
  }
  
  if(counter > 2) {
    while(counter >= 0) {
      B = preA;
      A = preB - quotient[counter] * preA;
      preB = B;
      preA = A;
      counter--;
    }
  } else {
    return 0;
  }
  if(A % mod < 0) {
    return A % mod + mod;
  }
  return A % mod;
}

// Compute Euler's Totient (assume n is product of two primes)
long totient(long n) {
  if(n == 0) {
    return 1;
  }
  long counter = 0;
  long i;
  for(i = 1; i <= n; i++) {
    if(gcd(i, n) == 1) {
      counter++;
    }
  }
  return counter;
}
