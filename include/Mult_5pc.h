#ifndef MULT_5PC_H_
#define MULT_5PC_H_

#include "NodeNetwork.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

inline bool chi_p_prime_in_T(int p_prime, int *T_map, uint n) {
    return (((((p_prime + 1 - 1) % n + 1) == T_map[0]) and (((p_prime + 2 - 1) % n + 1) == T_map[1])) or
            ((((p_prime + 1 - 1) % n + 1) == T_map[1]) and (((p_prime + 2 - 1) % n + 1) == T_map[0])));
}

inline bool p_prime_in_T(int p_prime, int *T_map) {
    return (p_prime == T_map[0] or p_prime == T_map[1]);
}



void test_prg(uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Mult_Bitwise_mp(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Mult_mp(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Mult_fixed_b_mp(Lint **c, Lint **a, Lint **b, uint b_index, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Mult_Byte_mp(uint8_t **c, uint8_t **a, uint8_t **b, uint size, NodeNetwork *nodeNet);
void Rss_MultPub_mp(Lint *c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);
void test_prg(uint size, uint ring_size, NodeNetwork *nodeNet);

#endif
