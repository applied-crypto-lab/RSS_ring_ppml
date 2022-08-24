/*   
   Multi-Party Replicated Secret Sharing over a Ring 
   ** Copyright (C) 2022 Alessandro Baccarini, Marina Blanton, and Chen Yuan
   ** Department of Computer Science and Engineering, University of Buffalo (SUNY)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
