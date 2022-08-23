#ifndef MULT_7PC_H_
#define MULT_7PC_H_

#include "NodeNetwork.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_Mult_7pc(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) ;
// inline bool chi_p_prime_in_T_7(int p_prime, int *T_map, uint n) ;
// inline bool p_prime_in_T_7(int p_prime, int *T_map) ;
inline bool chi_p_prime_in_T_7(int p_prime, int *T_map, uint n) {

    int chi_0 = (p_prime + 1 - 1) % n + 1;
    int chi_1 = (p_prime + 2 - 1) % n + 1;
    int chi_2 = (p_prime + 3 - 1) % n + 1;

    return ((chi_0 == T_map[0] or chi_0 == T_map[1] or chi_0 == T_map[2])
     and (chi_1 == T_map[0] or chi_1 == T_map[1] or chi_1 == T_map[2]) and 
     (chi_2 == T_map[0] or chi_2 == T_map[1] or chi_2 == T_map[2]));




    // for (uint8_t i = 0; i < 3; i++) {
    //     // if all the values are false, we can just return false because that means a match is not possible
    //     // if true, we continue checking each value of T_map
    //     if (!((chi_0 == T_map[i]) or (chi_1 == T_map[i]) or (chi_2 == T_map[i]))) {
    //         return false;
    //     }
    // }
    // we can only get here if matches are fouund in all three elements
    // return true;

}

inline bool p_prime_in_T_7(int p_prime, int *T_map) {
    return (p_prime == T_map[0] or p_prime == T_map[1] or p_prime == T_map[2]);
}



void Rss_MultPub_7pc(Lint *c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) ;


void Rss_Mult_Bitwise_7pc(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) ;
void Rss_Mult_Byte_7pc(uint8_t **c, uint8_t **a, uint8_t **b, uint size, NodeNetwork *nodeNet) ;


#endif
