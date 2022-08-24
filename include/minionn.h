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

#ifndef _MINIONN_H_
#define _MINIONN_H_
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "sys/time.h"
#include "3pc_all.h"
#include "neural_ops.h"
#include "svm_ops.h"
#include "init_io.h"
extern "C"{
#include "aes_ni.h"
}


// #include "benchmark_main.h"

void minionn(NodeNetwork*, NodeConfiguration*, string, uint, uint);



#endif
