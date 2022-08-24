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

#ifndef MSB_7PC_H
#define MSB_7PC_H

#include "Mult_7pc.h"
#include "Open_7pc.h"
#include "NodeNetwork.h"
#include "bitLT_7pc.h"
#include "randBit_7pc.h"
#include "edaBit_7pc.h"
#include "mpc_util.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void new_Rss_MSB_7pc(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) ;
void Rss_MSB_7pc(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) ;


#endif
