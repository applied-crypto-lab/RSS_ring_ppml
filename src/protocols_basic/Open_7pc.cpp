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

#include "Open_7pc.h"

void Rss_Open_7pc(Lint *res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) {
    uint threshold = nodeNet->getThreshold();
    // printf("threshold = %u\n", threshold);
    int i;
    Lint **recvbuf = new Lint *[threshold];
    Lint **sendbuf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recvbuf[i] = new Lint[size];
        memset(recvbuf[i], 0, sizeof(Lint) * size);
        sendbuf[i] = new Lint[size];
        memset(sendbuf[i], 0, sizeof(Lint) * size);
    }
    // printf("senfbuff start\n");
    for (i = 0; i < size; i++) {
        sendbuf[0][i] = a[0][i] + a[1][i] + a[2][i] + a[3][i] + a[4][i];
        sendbuf[1][i] = a[10][i] + a[11][i] + a[12][i] + a[13][i] + a[14][i];
        sendbuf[2][i] = a[1][i] + a[5][i] + a[16][i] + a[17][i] + a[18][i];
    }
    // printf("senfbuff end\n");

    nodeNet->SendAndGetDataFromPeer_Open(sendbuf, recvbuf, size, ring_size);
    // printf("received\n");
    for (i = 0; i < size; i++) {
        res[i] = a[0][i] + a[1][i] + a[2][i] + a[3][i] + a[4][i] + a[5][i] + a[6][i] + a[7][i] + a[8][i] + a[9][i] + a[10][i] + a[11][i] + a[12][i] + a[13][i] + a[14][i] + a[15][i] + a[16][i] + a[17][i] + a[18][i] + a[19][i];

        res[i] += recvbuf[0][i] + recvbuf[1][i] + recvbuf[2][i];

        res[i] = res[i] & nodeNet->SHIFT[ring_size];
    }
    // printf("end calc\n");

    for (i = 0; i < threshold; i++) {
        delete[] recvbuf[i];
        delete[] sendbuf[i];
    }
    delete[] recvbuf;
    delete[] sendbuf;
}
