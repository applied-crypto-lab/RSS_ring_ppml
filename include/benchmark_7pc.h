#ifndef _BENCHMARK_7PC
#define _BENCHMARK_7PC
#include "7pc_all.h"
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

void benchmark_7pc(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig, char *protocol, uint size, uint batch_size, uint num_iterations);

#endif
