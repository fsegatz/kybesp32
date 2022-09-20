idf_component_register(SRCS "kem.c"
                    INCLUDE_DIRS "." "../common"
                    REQUIRES "indcpa" "verify" "symmetric" "randombytes" "mbedtls")