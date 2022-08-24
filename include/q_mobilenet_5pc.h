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
