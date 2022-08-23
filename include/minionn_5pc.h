#ifndef _MINIONN_5PC_H_
#define _MINIONN_5PC_H_
#include "5pc_all.h"
#include "sys/time.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
extern "C" {
#include "aes_ni.h"
}

// #include "benchmark_main.h"

void minionn_mp(NodeNetwork *, NodeConfiguration *,string, uint, uint);

#endif
