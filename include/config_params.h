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

#ifndef CONFIG_PARAMS_H_
#define CONFIG_PARAMS_H_

#include <string>
#include <vector>
#include "setringsize.h"
using namespace std;

extern vector<string> alpha_string;
extern vector<string> input_image;
// extern string out_layer_path;
// extern string mobile_path;
// extern string labels_path;
// extern string model_structure;
// extern string interim_layer;
extern uint test_im; // which image we are testing

#endif