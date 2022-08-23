#ifndef NN_LAYERS_H_
#define NN_LAYERS_H_
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>

#include "NodeNetwork.h"
#include "Trunc.h"
#include "LT.h"
#include "matMult.h"
#include "sys/time.h"
#include "neural_ops.h"

void Rss_avg_pool_trunc(Lint **res, Lint **x, uint in_channels, uint *n, uint kernel, uint stride, uint batch_size, uint ring_size,  NodeNetwork *nodeNet, unsigned long &timer) ;

// neural network operations

// original
void im2col(Lint** res, Lint** a, uint channels, uint height, uint width, uint ksize, uint stride, uint batch_size,  NodeNetwork* nodeNet);

// tensorflow version
void extract_patch(Lint** res, Lint** data_im, int channels, int height, int width, int ksize, int stride, uint batch_size, int pad_flag, NodeNetwork* nodeNet);

void pad_original(Lint** res, Lint** data_im, int channels, int height, int width, int ksize, int stride, uint batch_size, NodeNetwork *nodeNet);



void Rss_depthwise_conv2d(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size,  NodeNetwork *nodeNet);
void Rss_pointwise_conv2d(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size,  NodeNetwork *nodeNet);


// void Rss_depthwise_conv2d(Lint **res, Lint **x, Lint **W, Lint**b, Lint **relu_bounds, uint *n, uint *in_channels, uint ring_size, uint batch_size, uint stride,  NodeNetwork *nodeNet);
// void Rss_pointwise_conv2d(Lint **res, Lint **x, Lint **W, Lint**b, Lint **relu_bounds, uint *n, uint *in_channels, uint out_channels, uint ring_size, uint batch_size,  NodeNetwork *nodeNet);

void Rss_dw_pw_conv2d(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size,  NodeNetwork *nodeNet, unsigned long &timer);

void Rss_conv2d(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint kernel, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size,  NodeNetwork *nodeNet) ;

void Rss_conv2d_first(Lint ***x, Lint **input_x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint kernel, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size,  NodeNetwork *nodeNet, unsigned long &timer) ;      

void Rss_conv2d_final_layer(Lint ***x,Lint **output_x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint kernel, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size,  NodeNetwork *nodeNet, unsigned long &timer) ;

void ReLU(Lint** res, Lint** a,  uint size, uint ring_size,  NodeNetwork* nodeNet, uint flag);
void old_ReLU(Lint** res, Lint** a,  uint size, uint ring_size,  NodeNetwork* nodeNet);
void eda_ReLU(Lint** res, Lint** a,  uint size, uint ring_size,  NodeNetwork* nodeNet);


void ReLU_6_alpha(Lint** res, Lint** x, Lint** upper, uint size, uint ring_size, uint layer,  NodeNetwork* nodeNet);
void ReLU_6_alpha_timer(Lint** res, Lint** x, Lint** upper, uint size, uint ring_size, uint layer,  NodeNetwork* nodeNet, unsigned long &timer);

void add_zp(Lint **res, Lint **x, Lint **z_p, uint size, uint batch_size, uint layer_id, NodeNetwork *nodeNet);
void sub_zp(Lint **res, Lint **x, Lint **z_p, uint size, uint batch_size, uint layer_id, NodeNetwork *nodeNet);



void MaxPool(Lint** res, Lint** a, uint c, uint m, uint n, uint batch_size, uint ring_sze,  NodeNetwork* nodeNet,uint flag);
void old_MaxPool(Lint** res, Lint** a, uint c, uint m, uint n, uint batch_size, uint ring_sze,  NodeNetwork* nodeNet);
void eda_MaxPool(Lint** res, Lint** a, uint c, uint m, uint n, uint batch_size, uint ring_sze,  NodeNetwork* nodeNet);

void add_biases(Lint** res, Lint **a, Lint **b, uint m, uint n, uint batch_size, NodeNetwork *nodeNet);
void pw_add_biases(Lint **res, Lint **x, Lint **b, uint height, uint width, uint channels, uint batch_size, NodeNetwork *nodeNet);



void conv(Lint** res, Lint **a, Lint **b, uint K, uint M, uint N, uint F, uint stride, uint batch_size, uint ring_size,  NodeNetwork* nodeNet);

void conv_dw(Lint** res, Lint **a, Lint **b, uint K, uint M, uint F, uint stride, uint batch_size, uint ring_size,  NodeNetwork* nodeNet);
void max_pool_bench(Lint** res, Lint **a, Lint **b, uint M, uint F, uint win, uint stride, uint batch_size, uint ring_size,  NodeNetwork* nodeNet);

void fire_module(Lint** res, Lint **a, Lint **b, uint M,  uint F, uint squeeze, uint expand, uint stride, uint batch_size, uint ring_size,  NodeNetwork* nodeNet);



#endif
