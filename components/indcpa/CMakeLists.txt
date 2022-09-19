idf_component_register(SRCS "indcpa.c"
                    INCLUDE_DIRS "." "../common"
                    REQUIRES "polyvec" "poly" "ntt" "symmetric" "randombytes")