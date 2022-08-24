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

#ifndef MATMULT_5PC_H_
#define MATMULT_5PC_H_

#include "NodeNetwork.h"
#include "Mult_5pc.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_MatMultArray_batch_mp(Lint **c, Lint **a, Lint **b, uint m, uint n, uint s, uint ring_size, uint batch_size, uint weight_flag_a, uint weight_flag_b,  NodeNetwork *nodeNet);
void Rss_dw_matrixmul_mp(Lint **res, Lint **x, Lint **W, uint width, uint height, uint channels, uint filter, uint ring_size, uint batch_size, uint stride,  NodeNetwork *nodeNet);
void Rss_pw_matrixmul_mp(Lint **res, Lint **x, Lint **W, uint width, uint height, uint in_channels, uint kernel, uint out_channels, uint ring_size, uint batch_size,  NodeNetwork *nodeNet);

#endif
