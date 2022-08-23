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
