#include <stdio.h>

#include "indcpa.h"
#include "randombytes.h"
#include "sha2.h"

void indcpa_keypair_gen(unsigned char *pk, unsigned char *sk) {
    
    uint8_t buf[64];
    //for(int i = 0; i<64; i++) buf[i] = 0;
    const uint8_t *publicseed = buf;
    const uint8_t *noiseseed = buf+32;

    esp_randombytes(buf, 32);
    sha512(buf, buf, 32);

    printf("-------------------\n");
    for(int i=0; i<64; i++) {
        printf("%i\t", buf[i]);
        if((i+1)%8==0) printf("\n");
    }



    return;
}