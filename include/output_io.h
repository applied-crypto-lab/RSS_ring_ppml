#ifndef OUTPUT_IO_H_
#define OUTPUT_IO_H_
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "sys/time.h"
#include "cmath"
#include "q_struct.h"
#include "config_params.h"
extern "C"{
#include "aes_ni.h"
}


void write_benchmark(string fname, vector<string> exp_names, vector<vector <double> > exp_times, vector<vector <double> > exp_ratios, uint ring_size);


#endif
