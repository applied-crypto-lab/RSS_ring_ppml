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

#ifndef NEURAL_OPS_5PC_H_
#define NEURAL_OPS_5PC_H_
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>

#include "LT_5pc.h"
#include "Trunc_5pc.h"
#include "NodeNetwork.h"
#include "sys/time.h"

void MaxPool_mp(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size, NodeNetwork *nodeNet, uint flag) ;
void old_MaxPool_mp(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size, NodeNetwork *nodeNet) ;
void eda_MaxPool_mp(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size, NodeNetwork *nodeNet) ;
void ReLU_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet, uint flag) ;
void old_ReLU_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) ;
void eda_ReLU_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) ;

void ReLU_6_alpha_timer_mp(Lint **res, Lint **x, Lint **upper, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer) ;
void ReLU_6_alpha_mp(Lint **res, Lint **x, Lint **upper, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet) ;

void Rss_avg_pool_trunc_mp(Lint **res, Lint **x, uint in_channels, uint *n, uint kernel, uint stride, uint batch_size, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer) ;



#endif
