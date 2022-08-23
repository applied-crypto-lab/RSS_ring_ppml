#ifndef TRUNC_5PC_H
#define TRUNC_5PC_H

#include "Mult_5pc.h"
#include "edaBit_5pc.h"
#include "NodeNetwork.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_truncPriv_mp(Lint **res, Lint **x, Lint **priv_m, Lint public_M, uint priv_m_index, uint size, uint ring_size,  NodeNetwork *nodeNet) ;
void Rss_truncPriv_time_mp(Lint **res, Lint **x, Lint **priv_m, Lint public_M, uint priv_m_index, uint size, uint ring_size,  NodeNetwork *nodeNet, unsigned long &timer) ;
void Rss_truncPr_1_mp(Lint **res, Lint **x, Lint m, uint size, uint ring_size,  NodeNetwork *nodeNet) ;
void Rss_truncPr_time_mp(Lint **res, Lint **x, Lint m, uint size, uint ring_size,  NodeNetwork *nodeNet, unsigned long &timer) ;

#endif
