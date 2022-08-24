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

#ifndef BITADD_H_
#define BITADD_H_

#include "NodeNetwork.h"
#include "Mult.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_nBitAdd(Lint **res, Lint **r_bitwise, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_BitAdd(Lint **res, Lint **a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_BitAdd(Lint **res, Lint *a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_CircleOpL(Lint **d, uint r_size, uint size, NodeNetwork *nodeNet);
void Rss_CircleOpL_Lint(Lint **d, uint r_size, uint size, NodeNetwork *nodeNet);
void CarryBuffer2(Lint **buffer, Lint **d, uint **index_array, uint size, uint k, uint numShares);
void CarryBuffer_Lint(Lint **a_prime, Lint **b_prime, Lint **d, uint **index_array, uint size, uint k, uint numShares);

#endif
