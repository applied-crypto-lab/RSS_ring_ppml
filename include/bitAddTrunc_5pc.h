#ifndef BITADDTRUNC_5PC_H 
#define BITADDTRUNC_5PC_H 

#include "NodeNetwork.h"
#include "Mult_5pc.h"
#include "mpc_util.h"
#include "bitAdd.h"
#include "bitAdd_5pc.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>
void Rss_nBitAdd_trunc_mp(Lint **res, Lint **carry, Lint **r_bitwise, uint ring_size, uint m, uint size, NodeNetwork *nodeNet);
void Rss_BitAdd_trunc_mp(Lint **res, Lint **carry, Lint **a, Lint **b, uint ring_size, uint m, uint size, NodeNetwork *nodeNet);


#endif
