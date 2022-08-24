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

#ifndef LT_H_
#define LT_H_

#include "MSB.h"
#include "Mult.h"
#include "NodeNetwork.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_LT(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);
void new_Rss_LT(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet);
void fixed_Rss_LT(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet);
void fixed_Rss_GT(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet);
void fixed_Rss_LT_time(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer);
void fixed_Rss_GT_time(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer);
void fixed_Rss_GT_LT_time(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer);

#endif
