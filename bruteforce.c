#include <stdio.h>
#include <string.h>
#include <math.h>

long coprime(long x);
long gcd(long num1, long num2);
long mod_inverse(long base, long m);
long isprime(long x);
long modulo(long a, long b, long c);
long encrypt(long msg, long key, long c);
char decrypt(long msg, long key, int c);

int main(){
char message[256];
long pubkey_e;// exponent of the public key
long pubkey_mod;// mod componenet of public key
printf("Enter the public key (exponent then modulo separated by space): \n");
fgets(message, 255, stdin);
char * _pubkey_e = strtok(message, " ");
char * _pubkey_mod = strtok(NULL, " ");
pubkey_e = atol(_pubkey_e);
pubkey_mod = atol(_pubkey_mod);
printf("The key is %d %d\n", pubkey_e, pubkey_mod);
long firstPrime, secondPrime, n, t, j, e, d, ciphertext,s;

long fact;
for (secondPrime = 2; secondPrime < pubkey_mod; secondPrime++){
fact = pubkey_mod%secondPrime;
firstPrime = pubkey_mod/secondPrime;
if(fact == 0){
if(isprime(firstPrime) == 1){
if(isprime(secondPrime) == 1){
//printf("The two numbers are %d and %d\n", firstPrime, secondPrime);
break;

}
}
}

}
t = (firstPrime - 1) * (secondPrime -1);
e = coprime(t);
d = mod_inverse(e,t);
printf("Primes are: %d %d\n", firstPrime, secondPrime);
printf("The totient is %d\n", t);
printf("The d is found out to be %d\n", d);
printf("Cracked private key is: %d, %d\n", d, pubkey_mod);
long counter = 0;
while (counter == 0){
printf("Enter a letter to decrypt or 'quit' to exit:\n");
fgets(message, 255, stdin);
ciphertext = atol(message);
if(strcmp(message, "quit\n") == 0){
counter = 1;
break;
	}
			s = modulo(ciphertext, d, pubkey_mod);
			if(s < 0) {s *= -1;}
			printf("This letter decrypted to %d\n",s);
			printf("The letter is %c\n", decrypt(ciphertext, d, pubkey_mod));
		
	}

return 0;

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



long coprime(long x) {
  long value = rand() % x;
  
  while((gcd(value, x) != 1) || (value < 0)) {
    value++;
  }

  return value;
}

long gcd(long num1, long num2) {
    if(num2 == 0) {
        return num1;
	}
	else {
		return gcd(num2, num1 % num2);
	}
}

long isprime(long x) {
    long i = 0;
    for(i = 2; i  < x; i++) {
        if((x % i) == 0)return 0;
    }
    return 1;
}


//Computes Math.mod(Math.pow(a, b), c), for large values of a, b, c
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

// Given an integer representing an ASCII value, encrypt it via RSA
long encrypt(long msg, long key, long c) {
  return modulo(msg, key, c);
}

char decrypt(long msg, long key, int c) {
    //printf("%d decrypted using key=%d,c=%d as %d", msg, key, c, modulo(msg, key, c));
    return modulo(msg, key, c);
}

