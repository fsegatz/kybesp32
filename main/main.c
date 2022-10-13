#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "indcpa.h"
#include "kem.h"
#include "taskpriorities.h"
#include "symmetric.h"
#include "randombytes.h"

TaskFunction_t test_kyber_kem(void *pvParameters) {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    while(1)
    {
        uint8_t pk[CRYPTO_PUBLICKEYBYTES];
        uint8_t sk[CRYPTO_SECRETKEYBYTES];
        uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
        uint8_t key_a[CRYPTO_BYTES];
        uint8_t key_b[CRYPTO_BYTES];

        esp_cpu_cycle_count_t tmp[4];

        #if(MEASURESHA == 1)
        esp_cpu_cycle_count_t tmp1[4];
        uint8_t bufg[2*KYBER_SYMBYTES];
        esp_randombytes(bufg, KYBER_SYMBYTES);
        tmp1[0] = esp_cpu_get_cycle_count();
        hash_g(bufg, bufg, KYBER_SYMBYTES);
        tmp1[1] = esp_cpu_get_cycle_count();

        uint8_t bufh[2*KYBER_SYMBYTES];
        esp_randombytes(bufh, KYBER_SYMBYTES);
        tmp1[2] = esp_cpu_get_cycle_count();
        hash_h(bufh, bufh, KYBER_SYMBYTES);
        tmp1[3] = esp_cpu_get_cycle_count();

        #if(SHA_ACC == 1)
        printf("Clock cycle count \"hash_g\" SHA-512 acc: %lu \n", tmp1[1]-tmp1[0]);
        printf("Clock cycle count \"hash_h\" SHA-256 acc: %lu \n", tmp1[3]-tmp1[2]);
        #else
        printf("Clock cycle count \"hash_g\" SHA-512 non acc: %lu \n", tmp1[1]-tmp1[0]);
        printf("Clock cycle count \"hash_h\" SHA-256 non acc: %lu \n", tmp1[3]-tmp1[2]);
        #endif

        #endif

        tmp[0] = esp_cpu_get_cycle_count();
        //Alice generates a public key
        crypto_kem_keypair(pk, sk);
        tmp[1] = esp_cpu_get_cycle_count();

        //Bob derives a secret key and creates a response
        crypto_kem_enc(ct, key_b, pk);
        tmp[2] = esp_cpu_get_cycle_count();

        //Alice uses Bobs response to get her shared key
        crypto_kem_dec(key_a, ct, sk);
        tmp[3] = esp_cpu_get_cycle_count();

        //printf("Clock cycle count \"Reference\": %u \n", tmp[0]);
        printf("Clock cycle count \"crypto_kem_keypair\": %lu \n", tmp[1]-tmp[0]);
        printf("Clock cycle count \"crypto_kem_enc\": %lu \n", tmp[2]-tmp[1]);
        printf("Clock cycle count \"crypto_kem_dec\": %lu \n", tmp[3]-tmp[2]);

        //Wait 5 seconds
        // fflush(stdout);
        // vTaskDelay(pdMS_TO_TICKS(5000));
        if(memcmp(key_a, key_b, CRYPTO_BYTES)) {
            printf("ERROR keys\n");
        }

        vTaskDelete(NULL);
    }
}

void app_main(void)
{
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreatePinnedToCore(
                    test_kyber_kem,       /* Function that implements the task. */
                    "NAME",          /* Text name for the task. */
                    20000,      /* Stack size in words, not bytes. */
                    ( void * ) 1,    /* Parameter passed into the task. */
                    MAIN_TASK_PRIORITY, /* Priority at which the task is created. */
                    &xHandle, /* Used to pass out the created task's handle. */
                    (BaseType_t) 0); /* Core ID */     

    if( xReturned == pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        //vTaskDelete( xHandle );
    }

    return;
}

