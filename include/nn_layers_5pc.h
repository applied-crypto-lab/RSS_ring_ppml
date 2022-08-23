#ifndef NN_LAYERS_5PC_H_
#define NN_LAYERS_5PC_H_
#include "LT_5pc.h"
#include "NodeNetwork.h"
#include "Trunc_5pc.h"
#include "matMult_5pc.h"
#include "neural_ops.h"
#include "neural_ops_5pc.h"
#include "sys/time.h"
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <string>

void Rss_depthwise_conv2d_mp(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet);
void Rss_pointwise_conv2d_mp(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet);
void Rss_dw_pw_conv2d_mp(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer);
void Rss_conv2d_first_mp(Lint ***x, Lint **input_x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint kernel, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer);
void Rss_conv2d_mp(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint kernel, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet);
void Rss_conv2d_final_layer_mp(Lint ***x, Lint **output_x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint kernel, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer);
void Rss_conv2d_no_act_mp(Lint **res, Lint **x, Lint **W, Lint **b, Lint **relu_bunds, uint *n, uint kernel, uint *in_channels, uint out_channels, uint stride, uint ring_size, uint batch_size, uint *layer_id, NodeNetwork *nodeNet);

#endif
