#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <time.h>

gmp_randstate_t state;


void randPrime(mpz_t prime, unsigned long int bits) {
    
    mpz_init_set_ui(prime, 1);
    mpz_mul_2exp(prime, prime, bits); // prime = 1 * 2^{1023}
    mpz_nextprime(prime, prime);
}

void findGenerator(mpz_t generator, mpz_t prime) {
	mpz_t res;
	mpz_inits(generator, res, NULL);
	while(true) {
		mpz_urandomm(generator, state, prime);  ///random < prime 
		mpz_powm(res, generator, prime, prime);
		if (mpz_cmp(res, generator) == 0) {
			break;
		}
	}
	mpz_clear(res);

}

int main() {
	mpz_t prime, generator, publicKeyOfA, privateKeyOfA;
     // init state
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    mpz_inits(privateKeyOfA, publicKeyOfA, NULL);
   	const unsigned long int bits = 1023;
    
   	// A part 
    randPrime(prime, bits);
    gmp_printf("===prime===\n%Zd\n", prime);
    findGenerator(generator, prime);
	gmp_printf("===generator===\n%Zd\n", generator);

	mpz_urandomm(privateKeyOfA, state, prime);
	mpz_powm(publicKeyOfA, generator, privateKeyOfA, prime);  // cal public key 

	gmp_printf("===publicKeyOfA===\n%Zd\n", publicKeyOfA);

	// B part
	

	// clean up
	mpz_clears(privateKeyOfA, publicKeyOfA, generator, prime, NULL);
    return 0;
}