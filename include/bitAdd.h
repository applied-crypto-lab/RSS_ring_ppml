#ifndef BITADD_H_
#define BITADD_H_

#include "NodeNetwork.h"
#include "Mult.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_nBitAdd(Lint **res, Lint **r_bitwise, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_BitAdd(Lint **res, Lint **a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_BitAdd(Lint **res, Lint *a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_CircleOpL(Lint **d, uint r_size, uint size, NodeNetwork *nodeNet);
void Rss_CircleOpL_Lint(Lint **d, uint r_size, uint size, NodeNetwork *nodeNet);
void CarryBuffer2(Lint **buffer, Lint **d, uint **index_array, uint size, uint k, uint numShares);
void CarryBuffer_Lint(Lint **a_prime, Lint **b_prime, Lint **d, uint **index_array, uint size, uint k, uint numShares);

#endif
