#ifndef Q_MOBILENET_MP_H_
#define Q_MOBILENET_MP_H_
#include "init_io.h"
#include "neural_ops_5pc.h"
#include "nn_layers_5pc.h"
#include "config_params.h"
#include "svm_ops_5pc.h"
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


void q_mobilenet_mp(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig, string mobile_path, uint size, uint batch_size, uint alpha_index, uint input_dim, uint num_discarded);

#endif
