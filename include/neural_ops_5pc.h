#ifndef NEURAL_OPS_5PC_H_
#define NEURAL_OPS_5PC_H_
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>

#include "LT_5pc.h"
#include "Trunc_5pc.h"
#include "NodeNetwork.h"
#include "sys/time.h"

void MaxPool_mp(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size, NodeNetwork *nodeNet, uint flag) ;
void old_MaxPool_mp(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size, NodeNetwork *nodeNet) ;
void eda_MaxPool_mp(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size, NodeNetwork *nodeNet) ;
void ReLU_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet, uint flag) ;
void old_ReLU_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) ;
void eda_ReLU_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) ;

void ReLU_6_alpha_timer_mp(Lint **res, Lint **x, Lint **upper, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer) ;
void ReLU_6_alpha_mp(Lint **res, Lint **x, Lint **upper, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet) ;

void Rss_avg_pool_trunc_mp(Lint **res, Lint **x, uint in_channels, uint *n, uint kernel, uint stride, uint batch_size, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer) ;



#endif
