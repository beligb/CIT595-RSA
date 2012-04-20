#include <stdio.h>
#include <string.h>
#include <math.h>

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
return 0;

}
