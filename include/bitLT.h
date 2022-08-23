#ifndef BITLT_H_
#define BITLT_H_

#include "NodeNetwork.h"
#include "Mult.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_BitLT(Lint **res, Lint *a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_CarryOut(Lint **res, Lint *a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_CarryOutAux(Lint **res, Lint **d, uint r_size, uint size, NodeNetwork *nodeNet);
void CarryBuffer(Lint **buffer, Lint **d, uint size, uint r_size);
void OptimalBuffer(Lint **buffer, Lint **d, uint size, uint r_size, NodeNetwork *nodeNet);
void new_Rss_CarryOutAux(Lint **res, Lint **d, uint ring_size, uint size, NodeNetwork *nodeNet);
void CircleOp(Lint **res, Lint *p, Lint *g, uint size, uint r_size);
void new_Rss_CarryOutAux_Lint(Lint **res, Lint **d, uint ring_size, uint size, NodeNetwork *nodeNet);

#endif
