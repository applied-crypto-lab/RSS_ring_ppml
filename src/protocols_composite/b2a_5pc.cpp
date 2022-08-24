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

#include "b2a_5pc.h"

void Rss_b2a_mp(Lint **res, Lint **a, uint ring_size, uint size, NodeNetwork *nodeNet) {
    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    uint i;
    Lint **b = new Lint *[numShares];
    Lint **b_2 = new Lint *[numShares];
    Lint **sum = new Lint *[numShares];

    Lint *c = new Lint[size];

    for (i = 0; i < numShares; i++) {
        b[i] = new Lint[size];
        b_2[i] = new Lint[size];
        sum[i] = new Lint[size];
    }
    Lint *ai = new Lint[numShares];
    memset(ai, 0, sizeof(Lint) * numShares);
        if (pid == 1) {
            ai[0] = 1;
        } else if (pid == 4) {
            ai[5] = 1; // party 4's share 6
        } else if (pid == 5) {
            ai[3] = 1; // parthy 5's share 4
        }

    // Lint a1 = 0;
    // Lint a2 = 0;
    // switch (pid) {
    // case 1:
    //     a1 = 1;
    //     a2 = 0;
    //     break;
    // case 2:
    //     a1 = 0;
    //     a2 = 0;
    //     break;
    // case 3:
    //     a1 = 0;
    //     a2 = 1;
    //     break;
    // }
    Rss_RandBit_mp(b, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++) {
            b_2[s][i] = GET_BIT(b[s][i], Lint(0));
            // b_2[1][i] = GET_BIT(b[1][i], Lint(0));

            sum[s][i] = (a[s][i] ^ b_2[s][i]);
            // sum[1][i] = (a[1][i] ^ b_2[1][i]);
        }
    }

    Rss_Open_Bitwise_mp(c, sum, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++) {
            res[s][i] = ai[s] * c[i] + b[s][i] - 2 * c[i] * b[s][i];
        }
        // res[1][i] = a2 * c[i] + b[1][i] - 2 * c[i] * b[1][i];
    }

    delete[] c;
    delete[] ai;

    for (i = 0; i < numShares; i++) {
        delete[] b[i];
        delete[] b_2[i];
        delete[] sum[i];
    }
    delete[] b;
    delete[] b_2;
    delete[] sum;
}