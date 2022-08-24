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

#include "randBit_7pc.h"

void Rss_RandBit_7pc(Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) {

    int pid = nodeNet->getID();
    uint i;
    uint bytes = (ring_size + 9) >> 3;
    // printf("bytes : %u\n", bytes );
    // printf("size : %u\n", size );
    uint numShares = nodeNet->getNumShares();

    Lint **u = new Lint *[numShares];
    Lint **a = new Lint *[numShares];
    Lint **d = new Lint *[numShares];

    for (i = 0; i < numShares; i++) {
        u[i] = new Lint[size];
        a[i] = new Lint[size];
        d[i] = new Lint[size];
    }
    Lint *e = new Lint[size];
    Lint *c = new Lint[size];
    uint8_t *buffer = new uint8_t[bytes * size];

    Lint *ai = new Lint[numShares];
    memset(ai, 0, sizeof(Lint) * numShares);
    if (pid == 1) {
        ai[0] = 1;
    } else if (pid == 5) {
        ai[19] = 1; // party 4's share 6
    } else if (pid == 6) {
        ai[16] = 1; // parthy 5's share 4
    } else if (pid == 7) {
        ai[10] = 1; // parthy 5's share 4
    } 

    for (size_t j = 0; j < numShares; j++) {
        nodeNet->prg_getrandom(j, bytes, size, buffer);
        for (i = 0; i < size; i++) {
            memcpy(u[j] + i, buffer + i * bytes, bytes);
        }
    }


    for (i = 0; i < size; i++) {
        // ensuring [a] is odd
        for (size_t s = 0; s < numShares; s++)
            a[s][i] = (u[s][i] << Lint(1)) + ai[s];
        // a[1][i] = (u[1][i] << Lint(1)) + a2;
    }

    // printf("Rss_MultPub_7pc\n");
    // squaring a
    Rss_MultPub_7pc(e, a, a, size, ring_size + 2, nodeNet); // ringsize+2
    // printf("Rss_MultPub_7pc end\n");
    rss_sqrt_inv(c, e, size, ring_size + 2);

    // effectively combines the two loops into one, eliminates d variable
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            b[s][i] = (c[i] * a[s][i] + ai[s]) >> Lint(1);
        // b[1][i] = (c[i] * a[1][i] + a2) >> (1);
    }

    // freeing up
    delete[] c;
    delete[] buffer;
    delete[] e;
    for (i = 0; i < numShares; i++) {
        delete[] d[i];
        delete[] a[i];
        delete[] u[i];
    }
    delete[] ai;
    delete[] d;
    delete[] a;
    delete[] u;
}
