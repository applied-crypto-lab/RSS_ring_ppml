#ifndef _EDA_RB_PREGEN_5PC
#define _EDA_RB_PREGEN_5PC
#include "5pc_all.h"
#include "output_io.h"
#include "init_io.h"
#include "init_io_5pc.h"
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

void eda_rb_pregen_5pc(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig,  uint num_iterations, uint batch_size, uint alpha_index, uint input_dim, uint num_discarded, string path);

#endif
