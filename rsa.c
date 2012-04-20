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
long encrypt(long msg, long key, long c) {
  return modulo(msg, key, c);
}

char decrypt(long msg, long key, int c) {
    printf("%d decrypted using key=%d,c=%d as %d", msg, key, c, modulo(msg, key, c));
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

int main() {
  srand(time(0));
  long a = 45;
  long b = 34;
  printf("Trying %ld and %ld and getting %ld\n", a, b, gcd(a, b));

  a = 124;
  b = 58;
  printf("Trying %ld and %ld and getting %ld\n", a, b, gcd(a, b));

  a = 0;
  b = 1;
  printf("Trying %ld and %ld and getting %ld\n", a, b, gcd(a, b));

  a = 1;
  b = 1;
  printf("Trying %ld and %ld and getting %ld\n", a, b, gcd(a, b));

  a = 10005;
  b = 555;
  printf("Trying %ld and %ld and getting %ld\n", a, b, gcd(a, b));

  a = 555;
  b = 10005;
  printf("Trying %ld and %ld and getting %ld\n", a, b, gcd(a, b));

  long x = 123;
  printf("Trying %ld and coprime %ld\n", x, coprime(x));

  x = 46;
  printf("Trying %ld and coprime %ld\n", x, coprime(x));

  x = 1;
  printf("Trying %ld and coprime %ld\n", x, coprime(x));

  x = 101;
  printf("Trying %ld and coprime %ld\n", x, coprime(x));

  x = 553;
  printf("Trying %ld and coprime %ld\n", x, coprime(x));

  long c = 45;
  long d = 34;
  long e = 32;
  printf("Trying %ld and %ld and %ld and getting %ld\n", c, d, e, modulo(c, d, e));

  c = 452342;
  d = 345423;
  e = 322134;
  printf("Trying %ld and %ld and %ld and getting %ld\n", c, d, e, modulo(c, d, e));

  c = 1908392;
  d = 3219412;
  e = 3221343;
  printf("Trying %ld and %ld and %ld and getting %ld\n", c, d, e, modulo(c, d, e));

  c = 45891123;
  d = 34;
  e = 32321;
  printf("Trying %ld and %ld and %ld and getting %ld\n", c, d, e, modulo(c, d, e));

  c = 0;
  d = 1;
  e = 4;
  printf("Trying %ld and %ld and %ld and getting %ld\n", c, d, e, modulo(c, d, e));

  long z = 24;
  printf("Trying %ld for Euler's Totient %ld\n", z, totient(z));

  z = 463;
  printf("Trying %ld and Euler's Totient %ld\n", z, totient(z));

  z = 143215;
  printf("Trying %ld and Euler's Totient %ld\n", z, totient(z));

  z = 1;
  printf("Trying %ld and Euler's Totient %ld\n", z, totient(z));

  z = 0;
  printf("Trying %ld and Euler's Totient %ld\n", z, totient(z));

  long r = 324535;
  long s = 234;
  printf("Trying %ld and %ld and modInv %ld\n", r, s, mod_inverse(r, s));

  r = 124;
  s = 58;
  printf("Trying %ld and %ld and modInv %ld\n", r, s, mod_inverse(r, s));

  r = 0;
  s = 0;
  printf("Trying %ld and %ld and modInv %ld\n", r, s, mod_inverse(r, s));

  r = 132849589;
  s = 132425;
  printf("Trying %ld and %ld and modInv %ld\n", r, s, mod_inverse(r, s));

  r = 1231231;
  s = 90412;
  printf("Trying %ld and %ld and modInv %ld\n", r, s, mod_inverse(r, s));

  long msg1 = 15;
  long msg2 = 43;
  long msg3 = 134;
  long msg4 = 2;
  long msg5 = 0;
  
  long prime = 173;
  long other = coprime(171);
  printf("Two primes %ld and %ld\n", prime, other);

  long n = prime * other;
  printf("My n %ld\n", n);

  long tot = totient(n);
  printf("This is the totient %ld\n", tot);

  long pickE = rand() % tot;
  while(gcd(pickE, tot) != 1) {
    pickE = rand() % tot;
  }
  printf("This is my public e %ld\n", pickE);

  long calcD = mod_inverse(pickE, tot);
  printf("This is my private d %ld\n", calcD);

  msg1 = 80;
  msg2 = 104;
  msg3 = 65;
  msg4 = 111;
  msg5 = 87;

  long encrypted = endecrypt(msg1, pickE, n);
  printf("This is msg1 %ld %c\n", msg1, (char)msg1);
  printf("This is msg1 encrypted %ld\n", encrypted);
  long decrypted = endecrypt(encrypted, calcD, n);
  printf("This is msg1 decrypted %ld %c\n", decrypted, (char)decrypted);

  encrypted = endecrypt(msg2, pickE, n);
  printf("This is msg2 %ld %c\n", msg2, (char)msg2);
  printf("This is msg2 encrypted %ld\n", encrypted);
  decrypted = endecrypt(encrypted, calcD, n);
  printf("This is msg2 decrypted %ld %c\n", decrypted, (char)decrypted);

  encrypted = endecrypt(msg3, pickE, n);
  printf("This is msg3 %ld %c\n", msg3, (char)msg3);
  printf("This is msg3 encrypted %ld\n", encrypted);
  decrypted = endecrypt(encrypted, calcD, n);
  printf("This is msg3 decrypted %ld %c\n", decrypted, (char)decrypted);

  encrypted = endecrypt(msg4, pickE, n);
  printf("This is msg4 %ld %c\n", msg4, (char)msg4);
  printf("This is msg4 encrypted %ld\n", encrypted);
  decrypted = endecrypt(encrypted, calcD, n);
  printf("This is msg4 decrypted %ld %c\n", decrypted, (char)decrypted);

  encrypted = endecrypt(msg5, pickE, n);
  printf("This is msg5 %ld %c\n", msg5, (char)msg5);
  printf("This is msg5 encrypted %ld\n", encrypted);
  decrypted = endecrypt(encrypted, calcD, n);
  printf("This is msg5 decrypted %ld %c\n", decrypted, (char)decrypted);

  int counter = 0;
  while(counter < 10) {
    long prime = getPrime();
    printf("I'm a prime! %ld\n", prime);
    counter++;
  }

  long k = 567;
  long l = 3;
  printf("This is M %ld\n", getM(k, l));
}

