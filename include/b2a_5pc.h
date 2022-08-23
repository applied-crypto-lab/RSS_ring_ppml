#ifndef B2A_5PC_H_ 
#define B2A_5PC_H_ 

#include "NodeNetwork.h"
#include "Mult_5pc.h"
#include "Open_5pc.h"
#include "mpc_util.h"
#include "randBit_5pc.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_b2a_mp(Lint **res, Lint **a, uint ring_size, uint size, NodeNetwork *nodeNet);

#endif
