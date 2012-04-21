#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int isPrime(long x) {
    long i = 0;
    for(i = 2; i  < x; i++) {
        if((x % i) == 0)return 0;
    }
    return 1;
}

void generatePrimeNumbers(long *m, long *n) {
    long maximum = *m > *n ? *m : *n;
    long *primeList = (long *)malloc(maximum * sizeof(long));
    long index = 1;
    long i = 3;
    primeList[0] = 2;
    while(index < maximum) {
        if (isPrime(i) == 1) {
            primeList[index] = i;
            index++;
            
        }
        i += 2; 
    }
    
    *m = primeList[*m - 1];
    *n = primeList[*n - 1];     
    free(primeList);
}

long totient( long n) {
    long *primeList  = ( long *)malloc(n * sizeof( long));
    primeList[0] = 2;
    long j = 0;
    long p;
    long q;
    long index = 1;
    long i = 2;
    for(i = 3; i < n; i +=2) {
        if(isPrime(i) == 1) {
            primeList[index] = i;
            index++;
        }
    }
    for(i = 0; i < index; i++) {
        for (j = 0 ; j  <index; j++) {
            if(primeList[i] * primeList[j] == n) {
                p = primeList[i];
                q = primeList[j];
                free(primeList);
                return (p - 1) * (q - 1);
            }
        }
    }
    free(primeList);
    return -1;
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
long encrypt(long msg, long key, long c) {
  return modulo(msg, key, c);
}

char decrypt(long msg, long key, int c) {
//    printf("%d decrypted using key=%d,c=%d as %d", msg, key, c, modulo(msg, key, c));
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

void generateKeys(long a, long b, long *e, long *d, long *c) {
  while((*c < 2) || (*c > 10000) || (*e < 2) || (*e > 10000) || (*d < 2) || (*d > 10000)) {
    *c = a * b;
    *e = coprime( (a - 1) * (b - 1));
    *d = mod_inverse(*e,  ((a - 1) * (b - 1)));
  }
}


