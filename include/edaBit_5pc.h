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

#ifndef EDABIT_5PC_H
#define EDABIT_5PC_H

#include "NodeNetwork.h"
#include "bitAddTrunc_5pc.h"
#include "b2a_5pc.h"
#include "bitAdd_5pc.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_edaBit_mp(Lint **r, Lint **b_2, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_GenerateRandomShares_mp(Lint **res, Lint **res_bitwise, uint ring_size, uint size, NodeNetwork *nodeNet);
void Rss_edaBit_trunc_mp(Lint **r, Lint **r_prime, Lint **r_km1, uint size, uint ring_size, uint m, NodeNetwork *nodeNet);
void Rss_GenerateRandomShares_trunc_mp(Lint **res, Lint **res_prime, Lint **res_bitwise, uint ring_size, uint m, uint size, NodeNetwork *nodeNet);

#endif
