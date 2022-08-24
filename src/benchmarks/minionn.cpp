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

#include "minionn.h"

void minionn(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig, string model_path, uint size, uint batch_size) {


    int i,  k;
    Lint fractional = 1000;

    uint correct = 0;
    // may not be needed
    // int ring_size = nodeNet->RING;
    int ring_size;
    if (8 * sizeof(Lint) != 64) {
        printf(ANSI_COLOR_RED "ERROR: sizeof(Lint) != 64.\nRecompile with unsigned long.\nExiting....\n" ANSI_COLOR_RESET);
        exit(0);
    }

    uint final_ring_size = 49;

    Lint *res = new Lint[100000];
    Lint *res2 = new Lint[100000];
    memset(res, 0, sizeof(Lint) * 100000);

    struct timeval start;
    struct timeval end;
    unsigned long timer = 0;


    printf("size : %u\n", size);
    printf("batch_size : %u\n", batch_size);


    // double *W1_in, *W2_in, *W3_in, *W4_in, *b1_in, *b2_in, *b3_in, *b4_in, *X_in, *Y_in;

    // convert to long long int? (for consistency)
    long long *W1_in = new long long[16 * 25];
    long long *W2_in = new long long[16 * 400];
    long long *W3_in = new long long[100 * 256];
    long long *W4_in = new long long[10 * 100];
    long long *b1_in = new long long[1 * 16];
    long long *b2_in = new long long[1 * 16];
    long long *b3_in = new long long[1 * 100];
    long long *b4_in = new long long[1 * 10];
    long long *Y_in = new long long[1 * 100000];
    long long *X_in = new long long[784 * 10000];
    // long long  **X_in = new long long *[100000];
    // for (i = 0; i < 100000; i++) {
    // 	X_in[i] = new long long [784];
    // }

    Lint **W1 = new Lint *[2];
    Lint **W2 = new Lint *[2];
    Lint **W3 = new Lint *[2];
    Lint **W4 = new Lint *[2];
    Lint **b1 = new Lint *[2];
    Lint **b2 = new Lint *[2];
    Lint **b3 = new Lint *[2];
    Lint **b4 = new Lint *[2];
    Lint **X = new Lint *[2];
    Lint **Y = new Lint *[2];

    // vars for intermediate calculations
    Lint **X0 = new Lint *[2];        // convolution
    Lint **X1 = new Lint *[2];        // matmult
    Lint **X1_prime = new Lint *[2];  // matmult
    Lint **X1_prime2 = new Lint *[2]; // matmult
    Lint **X2 = new Lint *[2]; // maxpool
    Lint **X3 = new Lint *[2]; // convolution
    Lint **X3p = new Lint *[2]; // convolution
    Lint **X4 = new Lint *[2]; // matmult
    Lint **X5 = new Lint *[2]; // maxpool
    Lint **X6 = new Lint *[2]; // matmult
    Lint **X7 = new Lint *[2]; // matmult

    for (i = 0; i < 2; i++) {
        W1[i] = new Lint [16*25];
        memset(W1[i],0,sizeof(Lint)*(16*25));
        W2[i] = new Lint [16*400];
        memset(W2[i],0,sizeof(Lint)*(16*400));
        W3[i] = new Lint [100*256];
        memset(W3[i],0,sizeof(Lint)*(100*256));
        W4[i] = new Lint [10*100];
        memset(W4[i],0,sizeof(Lint)*(10*100));
        b1[i] = new Lint [1*16];
        memset(b1[i],0,sizeof(Lint)*(1*16));
        b2[i] = new Lint [1*16];
        memset(b2[i],0,sizeof(Lint)*(1*16));
        b3[i] = new Lint [1*100];
        memset(b3[i],0,sizeof(Lint)*(1*100));
        b4[i] = new Lint [1*10];
        memset(b4[i],0,sizeof(Lint)*(1*10));

        X[i]  = new Lint [784*10000*batch_size];
        memset(X[i],0,sizeof(Lint)*(784*10000*batch_size));
        Y[i]  = new Lint [1*100000*batch_size];
        memset(Y[i],0,sizeof(Lint)*(1*100000*batch_size));

        X0[i] = new Lint[25 * 576 * batch_size];
        memset(X0[i], 0, sizeof(Lint) * (25 * 576 * batch_size));
        X1[i] = new Lint[16 * 24 * 24 * batch_size];
        memset(X1[i], 0, sizeof(Lint) * (16 * 24 * 24 * batch_size));

        X1_prime[i] = new Lint[16 * 24 * 24 * batch_size];
        memset(X1_prime[i], 0, sizeof(Lint) * (16 * 24 * 24 * batch_size));
        X1_prime2[i] = new Lint[16 * 24 * 24 * batch_size];
        memset(X1_prime2[i], 0, sizeof(Lint) * (16 * 24 * 24 * batch_size));

        X2[i] = new Lint[16 * 12 * 12 * batch_size];
        memset(X2[i], 0, sizeof(Lint) * (16 * 12 * 12 * batch_size));
        X3[i] = new Lint[400 * 64 * batch_size];
        memset(X3[i], 0, sizeof(Lint) * (400 * 64 * batch_size));

        X3p[i] = new Lint[400 * 64 * batch_size];
        memset(X3p[i], 0, sizeof(Lint) * (400 * 64 * batch_size));

        X4[i] = new Lint[16 * 8 * 8 * batch_size];
        memset(X4[i], 0, sizeof(Lint) * (16 * 8 * 8 * batch_size));
        X5[i] = new Lint[16 * 4 * 4 * batch_size];
        memset(X5[i], 0, sizeof(Lint) * (16 * 16 * batch_size));
        X6[i] = new Lint[100 * 1 * batch_size];
        memset(X6[i], 0, sizeof(Lint) * (100 * 1 * batch_size));
        X7[i] = new Lint[10 * 1 * batch_size];
        memset(X7[i], 0, sizeof(Lint) * (10 * 1 * batch_size));
    }

    // std::string fpath = "../model/MNIST_CNN_small/W1.csv";
    std::string fpath[10] = {model_path + "/MNIST_CNN_small/W1.csv",
                             model_path + "/MNIST_CNN_small/W2.csv",
                             model_path + "/MNIST_CNN_small/W3.csv",
                             model_path + "/MNIST_CNN_small/W4.csv",
                             model_path + "/MNIST_CNN_small/b1.csv",
                             model_path + "/MNIST_CNN_small/b2.csv",
                             model_path + "/MNIST_CNN_small/b3.csv",
                             model_path + "/MNIST_CNN_small/b4.csv",
                            //  model_path + "/MNIST_CNN_small/X.csv",
                             model_path + "/MNIST_CNN_small/X_copy.csv",
                             model_path + "/MNIST_CNN_small/Y.csv"};

    i = 0;
    // reading in all the data, multiplying by the fractional,
    // and storing it in an integer (rounding down)
    // DO NOT CHANGE ORDER!!!!!!!!!!!!!!!!!!!!!!
    readFile(W1_in, 16, 25, fractional, fpath[i]);
    i++;
    readFile(W2_in, 16, 400, fractional, fpath[i]);
    i++;
    readFile(W3_in, 100, 256, fractional, fpath[i]);
    i++;
    readFile(W4_in, 10, 100, fractional, fpath[i]);
    i++;
    readB(b1_in, 1, 16, fractional, fpath[i], 1);
    i++;
    readB(b2_in, 1, 16, fractional, fpath[i], 2);
    i++;
    readB(b3_in, 1, 100, fractional, fpath[i], 3);
    i++;
    readB(b4_in, 1, 10, fractional, fpath[i], 4);
    i++;
    // printf("hi\n");

    // readFile(X_in, 784, 100, fractional, fpath[i]); i++;
    // readFile(X_in, 784, 1, fractional, fpath[i]); i++;
    readX(X_in, size, 784, fpath[i]);
    i++;

    readFile(Y_in, 1, 10000, 1, fpath[i]);
    i++;

    // ring_size = 48;
    ring_size = 20;
    splitData(W1, W1_in, 16, 25, ring_size, nodeNet);
    splitData(b1, b1_in, 1, 16, ring_size, nodeNet);

    ring_size = 30;
    splitData(W2, W2_in, 16, 400, ring_size, nodeNet);
    splitData(b2, b2_in, 1, 16, ring_size, nodeNet);

    ring_size = 40;
    splitData(W3, W3_in, 100, 256, ring_size, nodeNet);
    splitData(b3, b3_in, 1, 100, ring_size, nodeNet);

    // ring_size = 49;
    splitData(W4, W4_in, 10, 100, final_ring_size, nodeNet);
    splitData(b4, b4_in, 1, 10, final_ring_size, nodeNet);
    splitData(Y, Y_in, 1, 10000, final_ring_size, nodeNet);


    // gettimeofday(&start,NULL); // start timer here

    for (k = 0; k < (size / batch_size); k++) {

        // printf("k: %u\n", k);
        ring_size = 20;
        // can change the 1 to whatever our batch size will be
        // trivial operation
        splitX(X, X_in, 784, batch_size, k, ring_size, nodeNet);
        // printf("splitX done\n");


        // BEGIN EXECUTION
        gettimeofday(&start, NULL); // start timer here

        // nodeNet->RING = ring_size;
        im2col(X0, X, 1, 28, 28, 5, 1, batch_size, nodeNet);
        // gettimeofday(&start,NULL); //start timer here

        Rss_MatMultArray_batch(X1, W1, X0, 16, 25, 576, ring_size, batch_size, 0, 1, nodeNet);
        // printf("Rss_MatMultArray_batch done\n");

        add_biases(X1, X1, b1, 16, 576, batch_size, nodeNet);

        ReLU(X1, X1, 16 * 576 * batch_size, ring_size, nodeNet, 1);
        MaxPool(X2, X1, 16, 24, 24, batch_size, ring_size, nodeNet, 1);
        // Rss_Convert(X2, X2, 16 * 144 * batch_size, ring_size, ring_size + 10, nodeNet);
        new_Rss_Convert(X2, X2, 16 * 144 * batch_size, ring_size, ring_size + 10, nodeNet);

        im2col(X3, X2, 16, 12, 12, 5, 1, batch_size, nodeNet);

        ring_size = 30;
        //W3 --> 30, X3 --> 20
        // W2 stays constant
        Rss_MatMultArray_batch(X4, W2, X3, 16, 400, 64, ring_size, batch_size, 0, 1, nodeNet);
        add_biases(X4, X4, b2, 16, 64, batch_size, nodeNet);

        ReLU(X4, X4, 16 * 64 * batch_size, ring_size, nodeNet, 1);
        MaxPool(X5, X4, 16, 8, 8, batch_size, ring_size, nodeNet, 1);
        new_Rss_Convert(X5, X5, 16 * 4 * 4 * batch_size, ring_size, ring_size + 10, nodeNet);

       

        ring_size = 40;
        Rss_MatMultArray_batch(X6, W3, X5, 100, 256, 1, ring_size, batch_size, 0, 1, nodeNet);
        add_biases(X6, X6, b3, 100, 1, batch_size, nodeNet);

        ReLU(X6, X6, 100 * 1 * batch_size, ring_size, nodeNet, 1);
        Rss_Convert(X6, X6, 100 * 1 * batch_size, ring_size, final_ring_size, nodeNet);

        Rss_MatMultArray_batch(X7, W4, X6, 10, 100, 1, final_ring_size, batch_size, 0, 1, nodeNet);
        add_biases(X7, X7, b4, 10, 1, batch_size, nodeNet);

        gettimeofday(&end, NULL); // stop timer here
        timer += 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

        arg_max(X1, X0, X7, final_ring_size, 10, batch_size, nodeNet);
        Rss_Open(res, X0, batch_size, final_ring_size, nodeNet);

        for (i = 0; i < batch_size; i++) {
            if ((res[i] + 1) == Y_in[k * batch_size + i]) {
                // printf("Correct prediction at %u\n", k * batch_size + i);
                correct += 1;
            } else {
                // printf("Incorrect prediction at %u\n", k*batch_size + i);
            }
        }

    }

    // gettimeofday(&end,NULL); // stop timer here
    // timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;

    // printf("total runtime for NN exeuction = %lf ms\n", (double)(timer * 0.001));
    // printf("average prediction time = %lf ms\n", (double)(timer * 0.001) / size);

    printf("[%s_3pc] [%u, %i, %u] [%.6lf ms, %lu bytes] \n", "minionn", final_ring_size, size,  batch_size,  (double)(timer * 0.001) / size, nodeNet->getCommunicationInBytes() / size);
    // printf("[%s_3pc] [%u, %i, %u, ] [%.6lf ms,  %.6lf] \n", "minionn", ring_size, size,  batch_size,  (double)(timer * 0.001) / size);
    // printf("\n %u, %lf\n\n", batch_size,  (double)(timer * 0.001) / size);

    // printf("\nruntime for SVM with data size %d = %.6lf ms\n", size, (double)(timer*0.001)); //double check time calculation is correct
    // printf("Average prediction time for data size %d = %.6lf ms\n", size*batch_size, (double)(timer*0.001)/(size*batch_size)); //double check time calculation is correct

    // printf("Accuracy : %f %\n", (float)correct / size * 100);

    // cleanup
    delete[] res;
    delete[] res2;
    // 1d arrays

    // 2d arrays
    for (i = 0; i < 2; i++) {
        delete[] W1[i];
        delete[] W2[i];
        delete[] W3[i];
        delete[] W4[i];
        delete[] b1[i];
        delete[] b2[i];
        delete[] b3[i];
        delete[] b4[i];
        delete[] X[i];
        delete[] Y[i];

        delete[] X0[i];
        delete[] X1[i];
        delete[] X1_prime[i];
        delete[] X1_prime2[i];
        delete[] X2[i];
        delete[] X3[i];
        delete[] X3p[i];
        delete[] X4[i];
        delete[] X5[i];
        delete[] X6[i];
        delete[] X7[i];
    }
    delete[] W1;
    delete[] W2;
    delete[] W3;
    delete[] W4;
    delete[] b1;
    delete[] b2;
    delete[] b3;
    delete[] b4;
    delete[] X;
    delete[] Y;

    delete[] X0;
    delete[] X1;
    delete[] X1_prime;
    delete[] X1_prime2;
    delete[] X2;
    delete[] X3;
    delete[] X3p;
    delete[] X4;
    delete[] X5;
    delete[] X6;
    delete[] X7;

    delete[] W1_in;
    delete[] W2_in;
    delete[] W3_in;
    delete[] W4_in;
    delete[] b1_in;
    delete[] b2_in;
    delete[] b3_in;
    delete[] b4_in;
    delete[] Y_in;
    delete[] X_in;

    // for(i = 0; i < 10000; i++){
    // 	delete [] X_in[i];

    // }
    // delete [] X_in;
}
