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

#include "randBit.h"

void Rss_RandBit(Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) {

    int pid = nodeNet->getID();
    uint i;
    uint bytes = (ring_size + 9) >> 3;
    // printf("bytes : %llu\n", bytes );
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
    } else if (pid == 3) {
        ai[numShares - 1] = 1;
    }

    nodeNet->prg_getrandom(0, bytes, size, buffer);
    for (i = 0; i < size; i++) {
        memcpy(u[0] + i, buffer + i * bytes, bytes);
    }
    nodeNet->prg_getrandom(1, bytes, size, buffer);
    for (i = 0; i < size; i++) {
        memcpy(u[1] + i, buffer + i * bytes, bytes);
    }

    for (i = 0; i < size; i++) {
        // ensuring [a] is odd
        for (size_t s = 0; s < numShares; s++)
            a[s][i] = (u[s][i] << Lint(1)) + ai[s];
        // a[1][i] = (u[1][i] << Lint(1)) + a2;
    }
    // squaring a
    Rss_MultPub(e, a, a, size, ring_size + 2, nodeNet); // ringsize+2
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
    delete[] d;
    delete[] a;
    delete[] ai;
    delete[] u;
}


void rss_sqrt_inv(Lint *c, Lint *e, uint size, uint ring_size) {

    Lint c1, c2, temp, d_;
    uint i, j;

    for (i = 0; i < size; i++) {
        c1 = Lint(1);
        c2 = Lint(1);
        d_ = Lint(4); // 100 - the first mask

        for (j = 2; j < ring_size - 1; j++) {
            temp = e[i] - (c1) * (c1);
            if (temp != Lint(0)) {
                // get the jth+1 bit of temp, place it in jth position, and add to c1
                c1 += (temp & (d_ << Lint(1))) >> Lint(1);
            }

            temp = Lint(1) - c1 * c2;
            // get the jth bit of temp and add it to c2
            c2 += temp & d_;
            d_ = d_ << Lint(1);
        }
        // last round for the inv portion
        temp = Lint(1) - c1 * c2;
        c[i] = c2 + (temp & d_);
    }
}
