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
	mpz_t prime, generator, publicKeyOfA;
     // init state
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    const unsigned long int bits = 1023;
    
    randPrime(prime, bits);
    gmp_printf("===prime===\n%Zd\n", prime);
    findGenerator(generator, prime);
	gmp_printf("===generator===\n%Zd\n", generator);

	// mpz_t test;
	// mpz_inits(test, NULL);
	// mpz_powm(test, generator, prime, prime);
	// if (mpz_cmp(test, generator) == 0) {
	// 	printf("true!\n");
	// }

    return 0;
}