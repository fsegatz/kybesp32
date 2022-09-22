#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "indcpa.h"
#include "kem.h"

TaskFunction_t test_kyber_kem(void *pvParameters) {
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );

    while(1)
    {
        uint8_t pk[CRYPTO_PUBLICKEYBYTES];
        uint8_t sk[CRYPTO_SECRETKEYBYTES];
        uint8_t ct[CRYPTO_CIPHERTEXTBYTES];
        uint8_t key_a[CRYPTO_BYTES];
        uint8_t key_b[CRYPTO_BYTES];

        esp_cpu_ccount_t tmp[4];

        tmp[0] = esp_cpu_get_ccount();
        //Alice generates a public key
        crypto_kem_keypair(pk, sk);
        tmp[1] = esp_cpu_get_ccount();

        //Bob derives a secret key and creates a response
        crypto_kem_enc(ct, key_b, pk);
        tmp[2] = esp_cpu_get_ccount();

        //Alice uses Bobs response to get her shared key
        crypto_kem_dec(key_a, ct, sk);
        tmp[3] = esp_cpu_get_ccount();

        //printf("Clock cycle count \"Reference\": %u \n", tmp[0]);
        printf("Clock cycle count \"crypto_kem_keypair\": %u \n", tmp[1]-tmp[0]);
        printf("Clock cycle count \"crypto_kem_enc\": %u \n", tmp[2]-tmp[1]);
        printf("Clock cycle count \"crypto_kem_dec\": %u \n", tmp[3]-tmp[2]);

        //Wait 5 seconds
        fflush(stdout);
        vTaskDelay(pdMS_TO_TICKS(5000));
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
                    configMAX_PRIORITIES, /* Priority at which the task is created. */
                    &xHandle, /* Used to pass out the created task's handle. */
                    (BaseType_t) 0); /* Core ID */     

    if( xReturned == pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        //vTaskDelete( xHandle );
    }

    return;
}

