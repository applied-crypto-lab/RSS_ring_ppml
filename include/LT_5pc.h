#ifndef LT_5PC_H
#define LT_5PC_H

#include "MSB_5pc.h"
#include "Mult_5pc.h"
#include "NodeNetwork.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_LT_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);
void new_Rss_LT_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);
void fixed_Rss_LT_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet);
void fixed_Rss_GT_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet);
void fixed_Rss_LT_time_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer);
void fixed_Rss_GT_time_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer);
void fixed_Rss_GT_LT_time_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer);

#endif
