#ifndef MATMULT_7PC_H_
#define MATMULT_7PC_H_

#include "NodeNetwork.h"
#include "Mult_7pc.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_MatMultArray_batch_7pc(Lint **c, Lint **a, Lint **b, uint m, uint n, uint s, uint ring_size, uint batch_size, uint weight_flag_a, uint weight_flag_b,  NodeNetwork *nodeNet);

#endif
