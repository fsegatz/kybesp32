idf_component_register(SRCS "symmetric-aes.c" "symmetric-shake.c"
                    INCLUDE_DIRS "." "../common"
                    REQUIRES "aes256ctr" "fips202")