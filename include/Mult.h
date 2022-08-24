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

#ifndef MULT_H_
#define MULT_H_

#include "NodeNetwork.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_Mult_Bitwise(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Mult(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Mult_fixed_b(Lint **c, Lint **a, Lint **b, uint b_index, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Mult_Byte(uint8_t **c, uint8_t **a, uint8_t **b, uint size, NodeNetwork *nodeNet);
void Rss_MultPub(Lint *c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);

#endif
