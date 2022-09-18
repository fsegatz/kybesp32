#include <stdio.h>

#include "indcpa.h"
#include "esp_random.h"

void indcpa_keypair_gen(unsigned char *pk, unsigned char *sk) {
    uint8_t buf[64];

    esp_fill_random(&buf, 64);          //second 32bits dont change

    printf("-------------------\n");
    for(int i=0; i<64; i++) {
        printf("%i\t", buf[i]);
        if((i+1)%8==0) printf("\n");
    }

    return;
}