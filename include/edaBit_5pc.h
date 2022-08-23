#ifndef EDABIT_5PC_H
#define EDABIT_5PC_H

#include "NodeNetwork.h"
#include "bitAddTrunc_5pc.h"
#include "b2a_5pc.h"
#include "bitAdd_5pc.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_edaBit_mp(Lint **r, Lint **b_2, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_GenerateRandomShares_mp(Lint **res, Lint **res_bitwise, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_edaBit_trunc_mp(Lint **r, Lint **r_prime, Lint **r_km1, uint size, uint ring_size, uint m, NodeNetwork *nodeNet);
void Rss_GenerateRandomShares_trunc_mp(Lint **res, Lint **res_prime, Lint **res_bitwise, uint ring_size, uint m, uint size, NodeNetwork *nodeNet);

#endif
