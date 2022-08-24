/*   
   Multi-Party Replicated Secret Sharing over a Ring 
   ** Copyright (C) 2022 Alessandro Baccarini, Marina Blanton, and Chen Yuan
   ** Department of Computer Science and Engineering, University of Buffalo (SUNY)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
