#ifndef MSB_5PC_H
#define MSB_5PC_H

#include "Mult_5pc.h"
#include "NodeNetwork.h"
#include "bitLT_5pc.h"
#include "randBit_5pc.h"
#include "edaBit_5pc.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void new_Rss_MSB_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) ;
void new_Rss_MSB_time_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer) ;
void Rss_MSB_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) ;


#endif
