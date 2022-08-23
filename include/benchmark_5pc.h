#ifndef _BENCHMARK_5PC
#define _BENCHMARK_5PC
#include "5pc_all.h"
#include "output_io.h"
#include "sys/time.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
extern "C" {
#include "aes_ni.h"
}

using namespace std;

void benchmark_5pc(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig, char *protocol, uint size, uint batch_size, uint num_iterations);

#endif
