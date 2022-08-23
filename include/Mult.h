#ifndef MULT_H_
#define MULT_H_

#include "NodeNetwork.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_Mult_Bitwise(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Mult(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Mult_fixed_b(Lint **c, Lint **a, Lint **b, uint b_index, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Mult_Byte(uint8_t **c, uint8_t **a, uint8_t **b, uint size, NodeNetwork *nodeNet);
void Rss_MultPub(Lint *c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);

#endif
