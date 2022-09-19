#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "indcpa.h"
#include "kem.h"

const uint32_t test_kyber_kem_stack = 20000; //10240 to small
const uint32_t test_kyber_kem_priority = configMAX_PRIORITIES;

void test_kyber_kem(void) {
    while(1) {
        uint8_t pk[CRYPTO_PUBLICKEYBYTES];
        uint8_t sk[CRYPTO_SECRETKEYBYTES];
        uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
        uint8_t key_a[CRYPTO_BYTES];
        uint8_t key_b[CRYPTO_BYTES];

        //Alice generates a public key
        crypto_kem_keypair(pk, sk);

        //Bob derives a secret key and creates a response
        crypto_kem_enc(ct, key_b, pk);

        //Alice uses Bobs response to get her shared key
        crypto_kem_dec(key_a, ct, sk);

        //Wait 5 seconds
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void app_main(void)
{
    xTaskCreatePinnedToCore(&test_kyber_kem, "Test kem task", test_kyber_kem_stack, NULL, test_kyber_kem_priority, NULL, 0);
    return;
}

