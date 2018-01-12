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
	mpz_t prime, generator, publicKeyOfA, privateKeyOfA, privateKeyOfB, maskingKeyOfB, ephemeralKey, cipher, maskOfA, invert_maskOfA, temp, decrypted;
	mpz_t msg;
	char* plain = "123456789";

     // init random state
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    // init var
    mpz_inits(privateKeyOfA, publicKeyOfA, privateKeyOfB, maskingKeyOfB, ephemeralKey, msg, cipher, maskOfA, invert_maskOfA, temp, decrypted, NULL);
   	// define precision
   	const unsigned long int bits = 1023;
    
   	// A part: key gen
    randPrime(prime, bits);
    gmp_printf("===prime===\n%Zd\n", prime);
    findGenerator(generator, prime);
	gmp_printf("===generator===\n%Zd\n", generator);

	mpz_urandomm(privateKeyOfA, state, prime);
	mpz_powm(publicKeyOfA, generator, privateKeyOfA, prime);  // cal public key 

	gmp_printf("===publicKeyOfA===\n%Zd\n", publicKeyOfA);

	// B part: send msg to A
	mpz_urandomm(privateKeyOfB, state, prime);
	mpz_powm(ephemeralKey, generator, privateKeyOfB, prime);  // cal ephemeral key: generator^{i} mod prime
	mpz_powm(maskingKeyOfB, publicKeyOfA, privateKeyOfB, prime);
	// encrypt msg
	mpz_init_set_str(msg, plain, 10);

	mpz_mul(cipher, msg, maskingKeyOfB);
    mpz_mod(cipher, cipher, prime);

	// A part: recv cipher from B
    mpz_powm(maskOfA, ephemeralKey, privateKeyOfA, prime);
    mpz_invert(invert_maskOfA, maskOfA, prime);
    mpz_mul(temp, invert_maskOfA, cipher);
    mpz_mod(decrypted, temp, prime);
    gmp_printf("===Plain text: ===\n%Zd\n", decrypted);

	// clean up
	mpz_clears(privateKeyOfA, publicKeyOfA, generator, prime, privateKeyOfB, maskingKeyOfB, ephemeralKey, cipher, maskOfA, invert_maskOfA, temp, decrypted, NULL);
    return 0;
}