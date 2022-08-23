#ifndef BITLT_5PC_H_
#define BITLT_5PC_H_

#include "Mult_5pc.h"
#include "NodeNetwork.h"
#include "bitLT.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_BitLT_mp(Lint **res, Lint *a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_CarryOut_mp(Lint **res, Lint *a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void OptimalBuffer_mp(Lint **buffer, Lint **d, uint size, uint r_size, NodeNetwork *nodeNet);
void new_Rss_CarryOutAux_mp(Lint **res, Lint **d, uint ring_size, uint size, NodeNetwork *nodeNet);

#endif
