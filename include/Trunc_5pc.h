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

#ifndef TRUNC_5PC_H
#define TRUNC_5PC_H

#include "Mult_5pc.h"
#include "edaBit_5pc.h"
#include "NodeNetwork.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_truncPriv_mp(Lint **res, Lint **x, Lint **priv_m, Lint public_M, uint priv_m_index, uint size, uint ring_size,  NodeNetwork *nodeNet) ;
void Rss_truncPriv_time_mp(Lint **res, Lint **x, Lint **priv_m, Lint public_M, uint priv_m_index, uint size, uint ring_size,  NodeNetwork *nodeNet, unsigned long &timer) ;
void Rss_truncPr_1_mp(Lint **res, Lint **x, Lint m, uint size, uint ring_size,  NodeNetwork *nodeNet) ;
void Rss_truncPr_time_mp(Lint **res, Lint **x, Lint m, uint size, uint ring_size,  NodeNetwork *nodeNet, unsigned long &timer) ;

#endif
