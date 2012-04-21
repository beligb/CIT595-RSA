#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "rsa.h"

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
    printf("The key is %ld %ld\n", pubkey_e, pubkey_mod);
    long firstPrime, secondPrime, t, e, d, ciphertext,s;
    
    long fact;
    for (secondPrime = 2; secondPrime < pubkey_mod; secondPrime++){
        fact = pubkey_mod%secondPrime;
        firstPrime = pubkey_mod/secondPrime;
        if(fact == 0){
            if(isPrime(firstPrime) == 1){
                if(isPrime(secondPrime) == 1){
                    break;
                    
                }
            }
        }
        
    }
    t = (firstPrime - 1) * (secondPrime -1);
    e = pubkey_e;
    d = mod_inverse(e,t);
    printf("Primes are: %ld %ld\n", firstPrime, secondPrime);
    printf("The totient is %ld\n", t);
    printf("The d is found out to be %ld\n", d);
    printf("Cracked private key is: %ld, %ld\n", d, pubkey_mod);

    while (1){
        printf("Enter a letter to decrypt or 'quit' to exit:\n");
        fgets(message, 255, stdin);
        ciphertext = atoi(message);
	if(ciphertext == 0) {
	  ciphertext = (int)message[0];
	}

        if(strcmp(message, "quit\n") == 0){
            break;
	}
        s = decrypt(ciphertext, d, pubkey_mod);

        if(atol(message) == 0) {
	  s = encrypt(ciphertext, e, pubkey_mod);
	  printf("This letter was encrypted to %ld\n", s);
	} else {
	  printf("This letter decrypted to %ld\n",s);
	  printf("The letter is %c\n", (int)s);
        }
    }
    
    return 0;
    
}

