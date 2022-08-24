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

#ifndef BITLT_H_
#define BITLT_H_

#include "NodeNetwork.h"
#include "Mult.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_BitLT(Lint **res, Lint *a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_CarryOut(Lint **res, Lint *a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_CarryOutAux(Lint **res, Lint **d, uint r_size, uint size, NodeNetwork *nodeNet);
void CarryBuffer(Lint **buffer, Lint **d, uint size, uint r_size);
void OptimalBuffer(Lint **buffer, Lint **d, uint size, uint r_size, NodeNetwork *nodeNet);
void new_Rss_CarryOutAux(Lint **res, Lint **d, uint ring_size, uint size, NodeNetwork *nodeNet);
void CircleOp(Lint **res, Lint *p, Lint *g, uint size, uint r_size);
void new_Rss_CarryOutAux_Lint(Lint **res, Lint **d, uint ring_size, uint size, NodeNetwork *nodeNet);

#endif
