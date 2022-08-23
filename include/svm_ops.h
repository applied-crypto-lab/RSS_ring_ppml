#ifndef SVM_OPS_H_
#define SVM_OPS_H_
#include <stdio.h>

#include "MSB.h"
#include "NodeNetwork.h"
#include "cmath"
#include "LT.h"
#include "edaBit.h"
#include "sys/time.h"

void maximum(Lint **res, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void eda_maximum(Lint **res, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);

void arg_max(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void arg_max_helper(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);

void eda_arg_max(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void eda_arg_max_helper(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);

void eda_arg_max_time(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet, unsigned long &timer);
void eda_arg_max_helper_time(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet, unsigned long &timer);

#endif
