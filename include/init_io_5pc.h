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

#ifndef INIT_IO_5PC_H_
#define INIT_IO_5PC_H_

#include "NodeNetwork.h"
#include "init_io.h"
#include "cmath"
#include "config_params.h"
#include "mpc_util.h"
#include "q_struct.h"
#include "sys/time.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

extern "C" {
#include "aes_ni.h"
}

void splitData_mp(Lint **output, long long *input, uint m, uint n, uint ring_size, NodeNetwork *nodeNet);
void splitData_mp(Lint **output, vector<sLint> input, uint size, uint ring_size, NodeNetwork *nodeNet);
void splitData_mp(Lint **output, vector<Lint> input, uint size, uint ring_size, NodeNetwork *nodeNet);
void splitData_X_mp(Lint **output, vector<Lint> input, uint size, uint batch_size, uint ring_size, NodeNetwork *nodeNet);
void splitData_mp(Lint **output, vector<quant_param> input, uint size, uint ring_size, NodeNetwork *nodeNet);
void generateData_mp(Lint **output, uint size, uint ring_size, NodeNetwork *nodeNet);
void splitData_X_mp(Lint **output, long long *input, uint size, uint batch_size, uint tracker, uint ring_size, NodeNetwork *nodeNet);

void q_fullSetup_mp(
    Lint **x,
    Lint ***weights,
    Lint ***biases,
    Lint **relu_bounds,
    Lint **trunc_vals,
    Lint **input_zp,   // (z_1)
    Lint **weights_zp, // (z_2)
    Lint **output_zp,  // (z_3)
    string model_path,
    string image_path,
    vector<string> all_names,
    vector<vector<int>> model_shapes,
    vector<vector<int>> bias_dim,
    double alpha,
    uint input_dim,
    Lint public_M,
    uint ring_size,
    uint batch_size,
    NodeNetwork *nodeNet);

#endif
