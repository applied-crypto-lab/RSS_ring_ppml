#ifndef SVM_OPS_5_PC_H_
#define SVM_OPS_5_PC_H_
#include <stdio.h>

#include "NodeNetwork.h"
#include "cmath"
#include "edaBit_5pc.h"
#include "LT_5pc.h"
#include "MSB_5pc.h"
#include "sys/time.h"

void maximum_mp(Lint **res, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void eda_maximum_mp(Lint **res, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void arg_max_mp(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void arg_max_helper_mp(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void eda_arg_max_mp(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void eda_arg_max_helper_mp(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void eda_arg_max_time_mp(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet, unsigned long &timer);
void eda_arg_max_helper_time_mp(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet, unsigned long &timer);

#endif
