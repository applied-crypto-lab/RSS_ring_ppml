#ifndef _EDA_RB_PREGEN_3PC
#define _EDA_RB_PREGEN_3PC
#include "3pc_all.h"
#include "output_io.h"
#include "init_io.h"
#include "sys/time.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>
extern "C" {
#include "aes_ni.h"
}

using namespace std;

void eda_rb_pregen_3pc(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig,  uint num_iterations, uint batch_size, uint alpha_index, uint input_dim, uint num_discarded, string path);

#endif
