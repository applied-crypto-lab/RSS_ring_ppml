#ifndef MSB_7PC_H
#define MSB_7PC_H

#include "Mult_7pc.h"
#include "Open_7pc.h"
#include "NodeNetwork.h"
#include "bitLT_7pc.h"
#include "randBit_7pc.h"
#include "edaBit_7pc.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void new_Rss_MSB_7pc(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) ;
void Rss_MSB_7pc(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) ;


#endif
