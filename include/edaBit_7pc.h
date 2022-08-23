#ifndef EDABIT_7PC_H
#define EDABIT_7PC_H

#include "NodeNetwork.h"
// #include "bitAddTrunc_7pc.h"
// #include "b2a_7pc.h"
#include "bitAdd_7pc.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_edaBit_7pc(Lint **r, Lint **b_2, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_GenerateRandomShares_7pc(Lint **res, Lint **res_bitwise, uint ring_size, uint size, NodeNetwork *nodeNet);

#endif
