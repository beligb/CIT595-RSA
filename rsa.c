#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
long endecrypt(long msg, long key, long c) {
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

long main() {
  srand(time(0));
  long a = 45;
  long b = 34;
  printf("Trying %d and %d and getting %d\n", a, b, gcd(a, b));

  a = 124;
  b = 58;
  printf("Trying %d and %d and getting %d\n", a, b, gcd(a, b));

  a = 0;
  b = 1;
  printf("Trying %d and %d and getting %d\n", a, b, gcd(a, b));

  a = 1;
  b = 1;
  printf("Trying %d and %d and getting %d\n", a, b, gcd(a, b));

  a = 10005;
  b = 555;
  printf("Trying %d and %d and getting %d\n", a, b, gcd(a, b));

  a = 555;
  b = 10005;
  printf("Trying %d and %d and getting %d\n", a, b, gcd(a, b));

  long x = 123;
  printf("Trying %d and coprime %d\n", x, coprime(x));

  x = 46;
  printf("Trying %d and coprime %d\n", x, coprime(x));

  x = 1;
  printf("Trying %d and coprime %d\n", x, coprime(x));

  x = 101;
  printf("Trying %d and coprime %d\n", x, coprime(x));

  x = 553;
  printf("Trying %d and coprime %d\n", x, coprime(x));

  long c = 45;
  long d = 34;
  long e = 32;
  printf("Trying %d and %d and %d and getting %d\n", c, d, e, modulo(c, d, e));

  c = 452342;
  d = 345423;
  e = 322134;
  printf("Trying %d and %d and %d and getting %d\n", c, d, e, modulo(c, d, e));

  c = 1908392;
  d = 3219412;
  e = 3221343;
  printf("Trying %d and %d and %d and getting %d\n", c, d, e, modulo(c, d, e));

  c = 45891123;
  d = 34;
  e = 32321;
  printf("Trying %d and %d and %d and getting %d\n", c, d, e, modulo(c, d, e));

  c = 0;
  d = 1;
  e = 4;
  printf("Trying %d and %d and %d and getting %d\n", c, d, e, modulo(c, d, e));

  long z = 24;
  printf("Trying %d for Euler's Totient %d\n", z, totient(z));

  z = 463;
  printf("Trying %d and Euler's Totient %d\n", z, totient(z));

  z = 143215;
  printf("Trying %d and Euler's Totient %d\n", z, totient(z));

  z = 1;
  printf("Trying %d and Euler's Totient %d\n", z, totient(z));

  z = 0;
  printf("Trying %d and Euler's Totient %d\n", z, totient(z));

  long r = 324535;
  long s = 234;
  printf("Trying %d and %d and modInv %d\n", r, s, mod_inverse(r, s));

  r = 124;
  s = 58;
  printf("Trying %d and %d and modInv %d\n", r, s, mod_inverse(r, s));

  r = 0;
  s = 0;
  printf("Trying %d and %d and modInv %d\n", r, s, mod_inverse(r, s));

  r = 132849589;
  s = 132425;
  printf("Trying %d and %d and modInv %d\n", r, s, mod_inverse(r, s));

  r = 1231231;
  s = 90412;
  printf("Trying %d and %d and modInv %d\n", r, s, mod_inverse(r, s));

  long msg1 = 15;
  long msg2 = 43;
  long msg3 = 134;
  long msg4 = 2;
  long msg5 = 0;
  
  long prime = 173;
  long other = coprime(171);
  printf("Two primes %d and %d\n", prime, other);

  long n = prime * other;
  printf("My n %d\n", n);

  long tot = totient(n);
  printf("This is the totient %d\n", tot);

  long pickE = rand() % tot;
  while(gcd(pickE, tot) != 1) {
    pickE = rand() % tot;
  }
  printf("This is my public e %d\n", pickE);

  long calcD = mod_inverse(pickE, tot);
  printf("This is my private d %d\n", calcD);

  long encrypted = endecrypt(msg1, pickE, n);
  printf("This is msg1 %d\n", msg1);
  printf("This is msg1 encrypted %d\n", encrypted);
  long decrypted = endecrypt(encrypted, calcD, n);
  printf("This is msg1 decrypted %d\n", decrypted);

  encrypted = endecrypt(msg2, pickE, n);
  printf("This is msg2 %d\n", msg2);
  printf("This is msg2 encrypted %d\n", encrypted);
  decrypted = endecrypt(encrypted, calcD, n);
  printf("This is msg2 decrypted %d\n", decrypted);

  encrypted = endecrypt(msg3, pickE, n);
  printf("This is msg3 %d\n", msg3);
  printf("This is msg3 encrypted %d\n", encrypted);
  decrypted = endecrypt(encrypted, calcD, n);
  printf("This is msg3 decrypted %d\n", decrypted);

  encrypted = endecrypt(msg4, pickE, n);
  printf("This is msg4 %d\n", msg4);
  printf("This is msg4 encrypted %d\n", encrypted);
  decrypted = endecrypt(encrypted, calcD, n);
  printf("This is msg4 decrypted %d\n", decrypted);

  encrypted = endecrypt(msg5, pickE, n);
  printf("This is msg5 %d\n", msg5);
  printf("This is msg5 encrypted %d\n", encrypted);
  decrypted = endecrypt(encrypted, calcD, n);
  printf("This is msg5 decrypted %d\n", decrypted);
}

