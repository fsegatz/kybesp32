idf_component_register(SRCS "poly.c"
                    INCLUDE_DIRS "." "../common"
                    REQUIRES "ntt" "reduce" "cbd" "symmetric")