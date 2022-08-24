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

#ifndef SVM_OPS_H_
#define SVM_OPS_H_
#include <stdio.h>

#include "MSB.h"
#include "NodeNetwork.h"
#include "cmath"
#include "LT.h"
#include "edaBit.h"
#include "sys/time.h"

void maximum(Lint **res, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void eda_maximum(Lint **res, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);

void arg_max(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void arg_max_helper(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);

void eda_arg_max(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);
void eda_arg_max_helper(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet);

void eda_arg_max_time(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet, unsigned long &timer);
void eda_arg_max_helper_time(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet, unsigned long &timer);

#endif
