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

#include "Mult_7pc.h"
// extern "C"{
// #include "../aes_ni.h"
// }

void Rss_Mult_7pc(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) {

    uint bytes = (ring_size + 7) >> 3;
    uint i, p_prime, T_index;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();

    Lint *v = new Lint[size];
    uint8_t prg_ctrs[20] = {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 3, 4, 4, 3};

    Lint **recv_buf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recv_buf[i] = new Lint[size];
        memset(recv_buf[i], 0, sizeof(Lint) * size);
    }

    uint8_t **buffer = new uint8_t *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[prg_ctrs[i] * bytes * size];
        nodeNet->prg_getrandom(i, bytes, prg_ctrs[i] * size, buffer[i]);

        // sanitizing destination (just in case)
            // printf("case )\n");
        memset(c[i], 0, sizeof(Lint) * size);
    }
    Lint z = Lint(0);
    uint tracker;
    for (i = 0; i < size; i++) {
        v[i] = a[0][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[6][i] + b[7][i] + b[8][i] + b[9][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[14][i] + b[15][i] + b[16][i] + b[17][i] + b[18][i] + b[19][i]) + a[1][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[6][i] + b[7][i] + b[8][i] + b[9][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[14][i] + b[15][i] + b[16][i] + b[17][i] + b[18][i] + b[19][i]) + a[2][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[6][i] + b[7][i] + b[8][i] + b[9][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[14][i] + b[15][i] + b[16][i] + b[17][i] + b[18][i] + b[19][i]) + a[3][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[6][i] + b[7][i] + b[8][i] + b[9][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[14][i] + b[15][i] + b[16][i] + b[17][i] + b[18][i] + b[19][i]) + a[4][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[15][i]) + a[5][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[6][i] + b[7][i] + b[8][i] + b[9][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[14][i] + b[15][i] + b[16][i] + b[17][i] + b[18][i] + b[19][i]) + a[6][i] * (b[2][i] + b[5][i] + b[7][i] + b[9][i] + b[11][i] + b[13][i]) + a[7][i] * (b[0][i] + b[4][i] + b[5][i] + b[6][i] + b[10][i] + b[11][i] + b[12][i]) + a[8][i] * (b[0][i] + b[4][i] + b[5][i] + b[10][i] + b[11][i] + b[16][i]) + a[9][i] * (b[1][i] + b[4][i] + b[7][i] + b[8][i] + b[10][i] + b[13][i]) + a[10][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[9][i]) + a[11][i] * (b[0][i] + b[1][i] + b[4][i] + b[6][i] + b[7][i] + b[8][i]) + a[12][i] * (b[0][i] + b[1][i] + b[2][i] + b[4][i] + b[5][i] + b[7][i]) + a[13][i] * (b[0][i] + b[4][i] + b[5][i] + b[6][i]) + a[14][i] * (b[2][i] + b[4][i] + b[5][i]) + a[15][i] * (b[1][i] + b[4][i]) + a[16][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[15][i]) + a[17][i] * (b[0][i] + b[1][i] + b[2][i]) + a[18][i] * (b[1][i] + b[8][i]) + a[19][i] * (b[0][i] + b[5][i] + b[6][i]);

        // printf("finished calculating v\n");
        for (p_prime = 1; p_prime < numParties + 1; p_prime++) {
            // printf("\n");
            for (T_index = 0; T_index < numShares; T_index++) {
                tracker = 0;
                if ((p_prime != (pid)) and (!(p_prime_in_T_7(p_prime, nodeNet->T_map_7pc[T_index]))) and (!(chi_p_prime_in_T_7(p_prime, nodeNet->T_map_7pc[T_index], numParties)))) {
                    memcpy(&z, buffer[T_index] + (i * prg_ctrs[T_index] + tracker) * bytes, bytes);
                    c[T_index][i] += z;
                    tracker++;
                } else if (
                    (p_prime == pid) and (!(chi_p_prime_in_T_7(pid, nodeNet->T_map_7pc[T_index], numParties)))) {
                    memcpy(&z, buffer[T_index] + (i * prg_ctrs[T_index] + tracker) * bytes, bytes);
                    c[T_index][i] += z;
                    v[i] -= z;

                    tracker++;
                }
            }
        }
    }
    // communication
    nodeNet->SendAndGetDataFromPeer_Mult(v, recv_buf, size, ring_size);
    // nodeNet->prg_getrandom(0, bytes, size, buffer);
    for (i = 0; i < size; i++) {
        c[19][i] = c[19][i] + recv_buf[0][i];
        c[16][i] = c[16][i] + recv_buf[1][i];
        c[10][i] = c[10][i] + recv_buf[2][i];

        c[0][i] = c[0][i] + v[i];
    }

    for (i = 0; i < threshold; i++) {
        delete[] recv_buf[i];
    }

    for (i = 0; i < numShares; i++) {
        delete[] buffer[i];
    }

    // free
    delete[] v;
    delete[] buffer;
    delete[] recv_buf;
}


void Rss_MultPub_7pc(Lint *c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) {

    uint bytes = (ring_size + 7) >> 3;
    uint i, j;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    // uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();

    // printf("numShares = %u\n",numShares);
    // printf("bytes = %u\n",bytes);

    Lint **send_buf = new Lint *[numParties];
    Lint **recv_buf = new Lint *[numParties];
    for (i = 0; i < numParties; i++) {
        send_buf[i] = new Lint[size];
        memset(send_buf[i], 0, sizeof(Lint) * size);
        recv_buf[i] = new Lint[size];
        memset(recv_buf[i], 0, sizeof(Lint) * size);
    }
    Lint *v = new Lint[size];

    // move to outside function
    Lint *ops;
    uint *prg_ctrs;
    int num_ops = 0;
    // printf("switch\n");
    switch (pid) {
        
    case 1:
        // printf("case 1\n");
        num_ops = 20;
        prg_ctrs = new uint[numShares]{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        // ops = new Lint[num_ops]{(1), (1), (1), (1), (1), (1)};
        ops = new Lint[num_ops];
        ops[0] = Lint(1);
        ops[1] = Lint(1);
        ops[2] = Lint(1);
        ops[3] = Lint(1);
        ops[4] = Lint(1);
        ops[5] = Lint(1);
        ops[6] = Lint(1);
        ops[7] = Lint(1);
        ops[8] = Lint(1);
        ops[9] = Lint(1);
        ops[10] = Lint(1);
        ops[11] = Lint(1);
        ops[12] = Lint(1);
        ops[13] = Lint(1);
        ops[14] = Lint(1);
        ops[15] = Lint(1);
        ops[16] = Lint(1);
        ops[17] = Lint(1);
        ops[18] = Lint(1);
        ops[19] = Lint(1);
        break;
    case 2:
        // printf("case 2\n");
        num_ops = 20;
        prg_ctrs = new uint[numShares]{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        // ops = new Lint[num_ops]{(1), (1), (1), (1), (1), (1)};
        ops = new Lint[num_ops];
        ops[0] = Lint(1);
        ops[1] = Lint(1);
        ops[2] = Lint(1);
        ops[3] = Lint(1);
        ops[4] = Lint(1);
        ops[5] = Lint(1);
        ops[6] = Lint(1);
        ops[7] = Lint(1);
        ops[8] = Lint(1);
        ops[9] = Lint(1);
        ops[10] = Lint(1);
        ops[11] = Lint(1);
        ops[12] = Lint(1);
        ops[13] = Lint(1);
        ops[14] = Lint(1);
        ops[15] = Lint(1);
        ops[16] = Lint(1);
        ops[17] = Lint(1);
        ops[18] = Lint(1);
        ops[19] = Lint(1);
        break;
    case 3:
        // printf("case 3\n");
        num_ops = 20;
        prg_ctrs = new uint[numShares]{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        // ops = new Lint[num_ops]{(-1), (-1), (1), (1), (1), (1), (1)};
        ops = new Lint[num_ops];
        ops[0] = Lint(1);
        ops[1] = Lint(1);
        ops[2] = Lint(1);
        ops[3] = Lint(1);
        ops[4] = Lint(1);
        ops[5] = Lint(1);
        ops[6] = Lint(1);
        ops[7] = Lint(1);
        ops[8] = Lint(1);
        ops[9] = Lint(1);
        ops[10] = Lint(1);
        ops[11] = Lint(1);
        ops[12] = Lint(1);
        ops[13] = Lint(1);
        ops[14] = Lint(1);
        ops[15] = Lint(1);
        ops[16] = Lint(1);
        ops[17] = Lint(1);
        ops[18] = Lint(1);
        ops[19] = Lint(1);
        break;
    case 4:
        // printf("case 4\n");
        num_ops = 22;
        prg_ctrs = new uint[numShares]{3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        // ops = new Lint[num_ops]{(-1), (-1), (-1), (-1), (-1), (-1), (1), (1), (1)};
        ops = new Lint[num_ops];
        ops[0] = Lint(-1);
        ops[1] = Lint(-1);
        ops[2] = Lint(-1);
        ops[3] = Lint(1);
        ops[4] = Lint(1);
        ops[5] = Lint(1);
        ops[6] = Lint(1);
        ops[7] = Lint(1);
        ops[8] = Lint(1);
        ops[9] = Lint(1);
        ops[10] = Lint(1);
        ops[11] = Lint(1);
        ops[12] = Lint(1);
        ops[13] = Lint(1);
        ops[14] = Lint(1);
        ops[15] = Lint(1);
        ops[16] = Lint(1);
        ops[17] = Lint(1);
        ops[18] = Lint(1);
        ops[19] = Lint(1);
        ops[20] = Lint(1);
        ops[21] = Lint(1);

        break;
    case 5:
        // printf("case 5\n");
        num_ops = 28;
        prg_ctrs = new uint[numShares]{3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        ops = new Lint[num_ops];
        // ops = new Lint[num_ops]{(-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1)};
        ops[0] = Lint(-1);
        ops[1] = Lint(-1);
        ops[2] = Lint(-1);
        ops[3] = Lint(-1);
        ops[4] = Lint(-1);
        ops[5] = Lint(-1);
        ops[6] = Lint(-1);
        ops[7] = Lint(-1);
        ops[8] = Lint(-1);
        ops[9] = Lint(-1);
        ops[10] = Lint(-1);
        ops[11] = Lint(-1);
        ops[12] = Lint(1);
        ops[13] = Lint(1);
        ops[14] = Lint(1);
        ops[15] = Lint(1);
        ops[16] = Lint(1);
        ops[17] = Lint(1);
        ops[18] = Lint(1);
        ops[19] = Lint(1);
        ops[20] = Lint(1);
        ops[21] = Lint(1);
        ops[22] = Lint(1);
        ops[23] = Lint(1);
        ops[24] = Lint(1);
        ops[25] = Lint(1);
        ops[26] = Lint(1);
        ops[27] = Lint(1);
        break;
    case 6:
        // printf("case 6\n");
        num_ops = 40;
        prg_ctrs = new uint[numShares]{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        ops = new Lint[num_ops];
        // ops = new Lint[num_ops]{(-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1)};
        ops[0] = Lint(-1);
        ops[1] = Lint(-1);
        ops[2] = Lint(-1);
        ops[3] = Lint(-1);
        ops[4] = Lint(-1);
        ops[5] = Lint(-1);
        ops[6] = Lint(-1);
        ops[7] = Lint(-1);
        ops[8] = Lint(-1);
        ops[9] = Lint(-1);
        ops[10] = Lint(-1);
        ops[11] = Lint(-1);
        ops[12] = Lint(-1);
        ops[13] = Lint(-1);
        ops[14] = Lint(-1);
        ops[15] = Lint(-1);
        ops[16] = Lint(-1);
        ops[17] = Lint(-1);
        ops[18] = Lint(-1);
        ops[19] = Lint(-1);
        ops[20] = Lint(-1);
        ops[21] = Lint(-1);
        ops[22] = Lint(-1);
        ops[23] = Lint(-1);
        ops[24] = Lint(-1);
        ops[25] = Lint(-1);
        ops[26] = Lint(-1);
        ops[27] = Lint(-1);
        ops[28] = Lint(-1);
        ops[29] = Lint(-1);
        ops[30] = Lint(1);
        ops[31] = Lint(1);
        ops[32] = Lint(1);
        ops[33] = Lint(1);
        ops[34] = Lint(1);
        ops[35] = Lint(1);
        ops[36] = Lint(1);
        ops[37] = Lint(1);
        ops[38] = Lint(1);
        ops[39] = Lint(1);
        break;

    case 7:
        // printf("case 7\n");
        num_ops = 60;
        prg_ctrs = new uint[numShares]{3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
        ops = new Lint[num_ops];
        // ops = new Lint[num_ops]{(-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1)};
        ops[0] = Lint(-1);
        ops[1] = Lint(-1);
        ops[2] = Lint(-1);
        ops[3] = Lint(-1);
        ops[4] = Lint(-1);
        ops[5] = Lint(-1);
        ops[6] = Lint(-1);
        ops[7] = Lint(-1);
        ops[8] = Lint(-1);
        ops[9] = Lint(-1);
        ops[10] = Lint(-1);
        ops[11] = Lint(-1);
        ops[12] = Lint(-1);
        ops[13] = Lint(-1);
        ops[14] = Lint(-1);
        ops[15] = Lint(-1);
        ops[16] = Lint(-1);
        ops[17] = Lint(-1);
        ops[18] = Lint(-1);
        ops[19] = Lint(-1);
        ops[20] = Lint(-1);
        ops[21] = Lint(-1);
        ops[22] = Lint(-1);
        ops[23] = Lint(-1);
        ops[24] = Lint(-1);
        ops[25] = Lint(-1);
        ops[26] = Lint(-1);
        ops[27] = Lint(-1);
        ops[28] = Lint(-1);
        ops[29] = Lint(-1);
        ops[30] = Lint(-1);
        ops[31] = Lint(-1);
        ops[32] = Lint(-1);
        ops[33] = Lint(-1);
        ops[34] = Lint(-1);
        ops[35] = Lint(-1);
        ops[36] = Lint(-1);
        ops[37] = Lint(-1);
        ops[38] = Lint(-1);
        ops[39] = Lint(-1);
        ops[40] = Lint(-1);
        ops[41] = Lint(-1);
        ops[42] = Lint(-1);
        ops[43] = Lint(-1);
        ops[44] = Lint(-1);
        ops[45] = Lint(-1);
        ops[46] = Lint(-1);
        ops[47] = Lint(-1);
        ops[48] = Lint(-1);
        ops[49] = Lint(-1);
        ops[50] = Lint(-1);
        ops[51] = Lint(-1);
        ops[52] = Lint(-1);
        ops[53] = Lint(-1);
        ops[54] = Lint(-1);
        ops[55] = Lint(-1);
        ops[56] = Lint(-1);
        ops[57] = Lint(-1);
        ops[58] = Lint(-1);
        ops[59] = Lint(-1);
        break;
    }
    // printf("switch end\n");


    uint8_t **buffer = new uint8_t *[numShares];
    Lint **v_a = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[bytes * size];
        v_a[i] = new Lint[size];
        nodeNet->prg_getrandom(i, bytes, size, buffer[i]);
        memcpy(v_a[i], buffer[i], bytes * size);
    }

    // Lint z = 0;
    uint sign_index = 0;
    // printf("calc begin\n");

    for (size_t i = 0; i < size; i++) {
        c[i] = a[0][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[6][i] + b[7][i] + b[8][i] + b[9][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[14][i] + b[15][i] + b[16][i] + b[17][i] + b[18][i] + b[19][i]) + a[1][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[6][i] + b[7][i] + b[8][i] + b[9][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[14][i] + b[15][i] + b[16][i] + b[17][i] + b[18][i] + b[19][i]) + a[2][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[6][i] + b[7][i] + b[8][i] + b[9][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[14][i] + b[15][i] + b[16][i] + b[17][i] + b[18][i] + b[19][i]) + a[3][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[6][i] + b[7][i] + b[8][i] + b[9][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[14][i] + b[15][i] + b[16][i] + b[17][i] + b[18][i] + b[19][i]) + a[4][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[15][i]) + a[5][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[6][i] + b[7][i] + b[8][i] + b[9][i] + b[10][i] + b[11][i] + b[12][i] + b[13][i] + b[14][i] + b[15][i] + b[16][i] + b[17][i] + b[18][i] + b[19][i]) + a[6][i] * (b[2][i] + b[5][i] + b[7][i] + b[9][i] + b[11][i] + b[13][i]) + a[7][i] * (b[0][i] + b[4][i] + b[5][i] + b[6][i] + b[10][i] + b[11][i] + b[12][i]) + a[8][i] * (b[0][i] + b[4][i] + b[5][i] + b[10][i] + b[11][i] + b[16][i]) + a[9][i] * (b[1][i] + b[4][i] + b[7][i] + b[8][i] + b[10][i] + b[13][i]) + a[10][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i] + b[9][i]) + a[11][i] * (b[0][i] + b[1][i] + b[4][i] + b[6][i] + b[7][i] + b[8][i]) + a[12][i] * (b[0][i] + b[1][i] + b[2][i] + b[4][i] + b[5][i] + b[7][i]) + a[13][i] * (b[0][i] + b[4][i] + b[5][i] + b[6][i]) + a[14][i] * (b[2][i] + b[4][i] + b[5][i]) + a[15][i] * (b[1][i] + b[4][i]) + a[16][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[15][i]) + a[17][i] * (b[0][i] + b[1][i] + b[2][i]) + a[18][i] * (b[1][i] + b[8][i]) + a[19][i] * (b[0][i] + b[5][i] + b[6][i]);
        // printf("inner loop begin\n");

        sign_index = 0;
        for (size_t j = 0; j < numShares; j++) {
            // printf("inner loop begin\n");

            for (size_t v_index = 0; v_index < prg_ctrs[j]; v_index++) {
                c[i] = c[i] + ops[sign_index] * v_a[j][i];
                sign_index++;
                // printf("sign_index = %u\n", sign_index);
            }
        }
    }

    // move data into buf
    // printf("moving data into buff\n");
    for (i = 1; i <= numParties; i++) {
        if (i == pid) {
            continue;
        }
        memcpy(send_buf[i - 1], c, sizeof(Lint) * size);
    }

    // printf("multicasting\n");
    nodeNet->multicastToPeers(send_buf, recv_buf, size, ring_size);
    // printf("moving data out buff\n");

    for (i = 0; i < size; i++) {
        for (j = 0; j < numParties; j++) {
            c[i] = c[i] + recv_buf[j][i]; // we can just add up all received messages, including the one from itself (which is zero from earlier)
        }
        c[i] = c[i] & nodeNet->SHIFT[ring_size];
    }
    // printf("end\n");

    for (i = 0; i < numParties; i++) {
        delete[] send_buf[i];
        delete[] recv_buf[i];
    }
    delete[] send_buf;
    delete[] recv_buf;

    for (i = 0; i < numShares; i++) {
        delete[] buffer[i];
        delete[] v_a[i];
    }
    delete[] v_a;
    delete[] buffer;

    // free
    delete[] v;
    delete[] ops;
    delete[] prg_ctrs;
}

void Rss_Mult_Bitwise_7pc(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) {

    uint bytes = (ring_size + 7) >> 3;
    uint i, p_prime, T_index;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();

    Lint *v = new Lint[size];
    uint8_t prg_ctrs[20] = {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 3, 4, 4, 3};

    Lint **recv_buf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recv_buf[i] = new Lint[size];
        memset(recv_buf[i], 0, sizeof(Lint) * size);
    }

    uint8_t **buffer = new uint8_t *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[prg_ctrs[i] * bytes * size];
        nodeNet->prg_getrandom(i, bytes, prg_ctrs[i] * size, buffer[i]);

        // sanitizing destination (just in case)
            // printf("case )\n");
        memset(c[i], 0, sizeof(Lint) * size);
    }
    Lint z = Lint(0);
    uint tracker;
    for (i = 0; i < size; i++) {
        v[i] =
            (a[0][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[7][i] ^ b[8][i] ^ b[9][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[14][i] ^ b[15][i] ^ b[16][i] ^ b[17][i] ^ b[18][i] ^ b[19][i])) ^
            (a[1][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[7][i] ^ b[8][i] ^ b[9][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[14][i] ^ b[15][i] ^ b[16][i] ^ b[17][i] ^ b[18][i] ^ b[19][i])) ^
            (a[2][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[7][i] ^ b[8][i] ^ b[9][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[14][i] ^ b[15][i] ^ b[16][i] ^ b[17][i] ^ b[18][i] ^ b[19][i])) ^
            (a[3][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[7][i] ^ b[8][i] ^ b[9][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[14][i] ^ b[15][i] ^ b[16][i] ^ b[17][i] ^ b[18][i] ^ b[19][i])) ^
            (a[4][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[15][i])) ^
            (a[5][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[7][i] ^ b[8][i] ^ b[9][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[14][i] ^ b[15][i] ^ b[16][i] ^ b[17][i] ^ b[18][i] ^ b[19][i])) ^
            (a[6][i] & (b[2][i] ^ b[5][i] ^ b[7][i] ^ b[9][i] ^ b[11][i] ^ b[13][i])) ^
            (a[7][i] & (b[0][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[10][i] ^ b[11][i] ^ b[12][i])) ^
            (a[8][i] & (b[0][i] ^ b[4][i] ^ b[5][i] ^ b[10][i] ^ b[11][i] ^ b[16][i])) ^
            (a[9][i] & (b[1][i] ^ b[4][i] ^ b[7][i] ^ b[8][i] ^ b[10][i] ^ b[13][i])) ^
            (a[10][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[9][i])) ^
            (a[11][i] & (b[0][i] ^ b[1][i] ^ b[4][i] ^ b[6][i] ^ b[7][i] ^ b[8][i])) ^
            (a[12][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[4][i] ^ b[5][i] ^ b[7][i])) ^
            (a[13][i] & (b[0][i] ^ b[4][i] ^ b[5][i] ^ b[6][i])) ^
            (a[14][i] & (b[2][i] ^ b[4][i] ^ b[5][i])) ^
            (a[15][i] & (b[1][i] ^ b[4][i])) ^
            (a[16][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[15][i])) ^
            (a[17][i] & (b[0][i] ^ b[1][i] ^ b[2][i])) ^
            (a[18][i] & (b[1][i] ^ b[8][i])) ^
            (a[19][i] & (b[0][i] ^ b[5][i] ^ b[6][i]));

        // printf("finished calculating v\n");
        for (p_prime = 1; p_prime < numParties + 1; p_prime++) {
            // printf("\n");
            for (T_index = 0; T_index < numShares; T_index++) {
                tracker = 0;
                if ((p_prime != (pid)) and (!(p_prime_in_T_7(p_prime, nodeNet->T_map_7pc[T_index]))) and (!(chi_p_prime_in_T_7(p_prime, nodeNet->T_map_7pc[T_index], numParties)))) {
                    memcpy(&z, buffer[T_index] + (i * prg_ctrs[T_index] + tracker) * bytes, bytes);
                    c[T_index][i] ^= z;
                    tracker++;
                } else if (
                    (p_prime == pid) and (!(chi_p_prime_in_T_7(pid, nodeNet->T_map_7pc[T_index], numParties)))) {
                    memcpy(&z, buffer[T_index] + (i * prg_ctrs[T_index] + tracker) * bytes, bytes);
                    c[T_index][i] ^= z;
                    v[i] ^= z;

                    tracker++;
                }
            }
        }
    }
    // communication
    nodeNet->SendAndGetDataFromPeer_Mult(v, recv_buf, size, ring_size);
    // nodeNet->prg_getrandom(0, bytes, size, buffer);
    for (i = 0; i < size; i++) {
        c[19][i] = c[19][i] ^ recv_buf[0][i];
        c[16][i] = c[16][i] ^ recv_buf[1][i];
        c[10][i] = c[10][i] ^ recv_buf[2][i];

        c[0][i] = c[0][i] ^ v[i];
    }

    for (i = 0; i < threshold; i++) {
        delete[] recv_buf[i];
    }

    for (i = 0; i < numShares; i++) {
        delete[] buffer[i];
    }

    // free
    delete[] v;
    delete[] buffer;
    delete[] recv_buf;
}

void Rss_Mult_Byte_7pc(uint8_t **c, uint8_t **a, uint8_t **b, uint size, NodeNetwork *nodeNet) {

    uint i, p_prime, T_index;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();

    uint8_t *v = new uint8_t[size];
    uint8_t prg_ctrs[20] = {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 3, 4, 4, 3};

    uint8_t **recv_buf = new uint8_t *[threshold];
    for (i = 0; i < threshold; i++) {
        recv_buf[i] = new uint8_t[size];
        memset(recv_buf[i], 0, sizeof(uint8_t) * size);
    }

    uint8_t **buffer = new uint8_t *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[prg_ctrs[i] * size];
        nodeNet->prg_getrandom(i, 1, prg_ctrs[i] * size, buffer[i]);
        // sanitizing destination (just in case)
            // printf("case )\n");
        memset(c[i], 0, sizeof(uint8_t) * size);
    }
    uint tracker;
    uint trackers[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (i = 0; i < size; i++) {
        v[i] =
            (a[0][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[7][i] ^ b[8][i] ^ b[9][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[14][i] ^ b[15][i] ^ b[16][i] ^ b[17][i] ^ b[18][i] ^ b[19][i])) ^
            (a[1][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[7][i] ^ b[8][i] ^ b[9][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[14][i] ^ b[15][i] ^ b[16][i] ^ b[17][i] ^ b[18][i] ^ b[19][i])) ^
            (a[2][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[7][i] ^ b[8][i] ^ b[9][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[14][i] ^ b[15][i] ^ b[16][i] ^ b[17][i] ^ b[18][i] ^ b[19][i])) ^
            (a[3][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[7][i] ^ b[8][i] ^ b[9][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[14][i] ^ b[15][i] ^ b[16][i] ^ b[17][i] ^ b[18][i] ^ b[19][i])) ^
            (a[4][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[15][i])) ^
            (a[5][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[7][i] ^ b[8][i] ^ b[9][i] ^ b[10][i] ^ b[11][i] ^ b[12][i] ^ b[13][i] ^ b[14][i] ^ b[15][i] ^ b[16][i] ^ b[17][i] ^ b[18][i] ^ b[19][i])) ^
            (a[6][i] & (b[2][i] ^ b[5][i] ^ b[7][i] ^ b[9][i] ^ b[11][i] ^ b[13][i])) ^
            (a[7][i] & (b[0][i] ^ b[4][i] ^ b[5][i] ^ b[6][i] ^ b[10][i] ^ b[11][i] ^ b[12][i])) ^
            (a[8][i] & (b[0][i] ^ b[4][i] ^ b[5][i] ^ b[10][i] ^ b[11][i] ^ b[16][i])) ^
            (a[9][i] & (b[1][i] ^ b[4][i] ^ b[7][i] ^ b[8][i] ^ b[10][i] ^ b[13][i])) ^
            (a[10][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i] ^ b[9][i])) ^
            (a[11][i] & (b[0][i] ^ b[1][i] ^ b[4][i] ^ b[6][i] ^ b[7][i] ^ b[8][i])) ^
            (a[12][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[4][i] ^ b[5][i] ^ b[7][i])) ^
            (a[13][i] & (b[0][i] ^ b[4][i] ^ b[5][i] ^ b[6][i])) ^
            (a[14][i] & (b[2][i] ^ b[4][i] ^ b[5][i])) ^
            (a[15][i] & (b[1][i] ^ b[4][i])) ^
            (a[16][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[15][i])) ^
            (a[17][i] & (b[0][i] ^ b[1][i] ^ b[2][i])) ^
            (a[18][i] & (b[1][i] ^ b[8][i])) ^
            (a[19][i] & (b[0][i] ^ b[5][i] ^ b[6][i]));
        // printf("finished calculating v\n");
        for (p_prime = 1; p_prime < numParties + 1; p_prime++) {
            // printf("\n");
            for (T_index = 0; T_index < numShares; T_index++) {
                tracker = 0;
                if ((p_prime != (pid)) and (!(p_prime_in_T_7(p_prime, nodeNet->T_map_7pc[T_index]))) and (!(chi_p_prime_in_T_7(p_prime, nodeNet->T_map_7pc[T_index], numParties)))) {
                    c[T_index][i] ^= buffer[T_index][trackers[T_index]];
                    tracker++;
                } else if (
                    (p_prime == pid) and (!(chi_p_prime_in_T_7(pid, nodeNet->T_map_7pc[T_index], numParties)))) {
                    c[T_index][i] = c[T_index][i] ^ buffer[T_index][trackers[T_index]];

                    v[i] ^= buffer[T_index][trackers[T_index]];

                    tracker++;
                }
            }
        }
    }
    // communication
    nodeNet->SendAndGetDataFromPeer_bit_Mult(v, recv_buf, size);
    for (i = 0; i < size; i++) {
        c[19][i] = c[19][i] ^ recv_buf[0][i];
        c[16][i] = c[16][i] ^ recv_buf[1][i];
        c[10][i] = c[10][i] ^ recv_buf[2][i];

        c[0][i] = c[0][i] ^ v[i];
    }

    for (i = 0; i < threshold; i++) {
        delete[] recv_buf[i];
    }

    for (i = 0; i < numShares; i++) {
        delete[] buffer[i];
    }

    // free
    delete[] v;
    delete[] buffer;
    delete[] recv_buf;
}