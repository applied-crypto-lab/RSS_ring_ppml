/*   
   Multi-Party Replicated Secret Sharing over a Ring 
   ** Copyright (C) 2022 Alessandro Baccarini, Marina Blanton, and Chen Yuan
   ** Department of Computer Science and Engineering, University of Buffalo (SUNY)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
