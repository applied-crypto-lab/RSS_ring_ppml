#ifndef Q_MOBILENET_H_
#define Q_MOBILENET_H_
#include "init_io.h"
#include "neural_ops.h"
#include "nn_layers.h"
#include "config_params.h"
#include "svm_ops.h"
#include "sys/time.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <sstream>
#include <vector>
extern "C" {
#include "aes_ni.h"
}

void q_mobilenet(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig, string mobile_path, uint size, uint batch_size, uint alpha_index, uint input_dim, uint num_discarded);


#endif
