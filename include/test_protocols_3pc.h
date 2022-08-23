#ifndef _TEST_PROTOCOLS_3PC
#define _TEST_PROTOCOLS_3PC
#include "3pc_all.h"
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

void test_protocols_3pc(NodeNetwork *, NodeConfiguration *, uint, uint);

#endif
