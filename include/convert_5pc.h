#ifndef CONVERT_5PC_H_ 
#define CONVERT_5PC_H_ 

#include "NodeNetwork.h"
#include "Mult_5pc.h"
#include "Open_5pc.h"
#include "mpc_util.h"
#include "randBit_5pc.h"
#include "edaBit_5pc.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void new_Rss_Convert_mp(Lint **res, Lint **a, uint size, uint ring_size, uint ring_size_prime,  NodeNetwork *nodeNet);
void Rss_Convert_mp(Lint **res, Lint **a, uint size, uint ring_size, uint ring_size_prime,  NodeNetwork *nodeNet);


#endif
