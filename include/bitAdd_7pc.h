#ifndef BITADD_5PC_H 
#define BITADD_5PC_H 

#include "NodeNetwork.h"
#include "Mult_7pc.h"
#include "mpc_util.h"
#include "bitAdd.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_nBitAdd_7pc(Lint **res, Lint **r_bitwise, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_BitAdd_7pc(Lint **res, Lint **a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_BitAdd_7pc(Lint **res, Lint *a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_CircleOpL_7pc(Lint **d, uint r_size, uint size, NodeNetwork *nodeNet);
void Rss_CircleOpL_7pc_test(Lint **d, uint r_size, uint size, NodeNetwork *nodeNet);
void CarryBuffer2_test(Lint **buffer, Lint **d, uint **index_array, uint size, uint k, uint numShares);
void Rss_CircleOpL_Lint_7pc(Lint **d, uint r_size, uint size, NodeNetwork *nodeNet);


#endif
