#ifndef _MINIONN_H_
#define _MINIONN_H_
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "sys/time.h"
#include "3pc_all.h"
#include "neural_ops.h"
#include "svm_ops.h"
#include "init_io.h"
extern "C"{
#include "aes_ni.h"
}


// #include "benchmark_main.h"

void minionn(NodeNetwork*, NodeConfiguration*, string, uint, uint);



#endif
