#ifndef MATMULT_5PC_H_
#define MATMULT_5PC_H_

#include "NodeNetwork.h"
#include "Mult_5pc.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_MatMultArray_batch_mp(Lint **c, Lint **a, Lint **b, uint m, uint n, uint s, uint ring_size, uint batch_size, uint weight_flag_a, uint weight_flag_b,  NodeNetwork *nodeNet);
void Rss_dw_matrixmul_mp(Lint **res, Lint **x, Lint **W, uint width, uint height, uint channels, uint filter, uint ring_size, uint batch_size, uint stride,  NodeNetwork *nodeNet);
void Rss_pw_matrixmul_mp(Lint **res, Lint **x, Lint **W, uint width, uint height, uint in_channels, uint kernel, uint out_channels, uint ring_size, uint batch_size,  NodeNetwork *nodeNet);

#endif
