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

#include "main.h"

NodeConfiguration *nodeConfig;
NodeNetwork *nNet;

int main(int argc, char **argv) {
    printf("8*sizeof(Lint) = %lu\n", 8 * sizeof(Lint));
    printf("sizeof(Lint) = %lu\n", sizeof(Lint));

    if (argc < 4) {
        fprintf(stderr, "Incorrect input parameters\n");
        // fprintf(stderr, "Usage: <id> <runtime-config> <size of data(2^x)> <ring size(x bits)> <batch_size>\n");
        fprintf(stderr, "Usage: <id> <runtime-config> <experiment>\n");
        fprintf(stderr, "experiments: micro, minionn, q_mobilenet, test\n");
        exit(1);
    }
    nodeConfig = new NodeConfiguration(atoi(argv[1]), argv[2], 128);
    std::cout << "Creating the NodeNetwork\n";
    printf("peer count = %u\n", nodeConfig->getPeerCount());
    uint numPeers = nodeConfig->getPeerCount();

    if (!strcmp(argv[3], "micro")) {
        if (argc < 9) {
            fprintf(stderr, "Incorrect input parameters for microbenchmark\n");
            fprintf(stderr, "Usage: <id> <runtime-config> micro <protocol> <ring size(x bits)> <size of data(2^x)> <batch_size> <num_iterations>\n");
            fprintf(stderr, "protocols: mult, mulpub, randbit, edabit, msb_rb, msb_eda, mat_mult\n");
            fprintf(stderr, "matrix sizes: 100, 10000, 250000, 1000000\n");
            delete nodeConfig;
            exit(1);
        }
        uint num_parties = numPeers + 1;
        uint threshold = num_parties / 2;
        uint ring_size = atoi(argv[5]);
        uint size = atoi(argv[6]);
        uint batch_size = atoi(argv[7]);
        uint num_iterations = atoi(argv[8]);
        cout << "num_parties = " << num_parties << endl;
        cout << "threshold = " << threshold << endl;
        cout << "ring_size = " << ring_size << endl;
        cout << "size = " << size << endl;
        cout << "batch_size = " << batch_size << endl;
        cout << "num_iterations = " << num_iterations << endl;

        nNet = new NodeNetwork(nodeConfig, atoi(argv[1]), 1, ring_size, num_parties, threshold);
        benchmark_main(num_parties, argv[4], size, batch_size, num_iterations);

    } else if (!strcmp(argv[3], "minionn")) {
        if (argc < 7) {
            fprintf(stderr, "Incorrect input parameters for minionn\n");
            fprintf(stderr, "Usage: <id> <runtime-config> minionn <num_images> <batch_size> <model_path>\n");
            delete nodeConfig;
            exit(1);
        }
        uint num_parties = numPeers + 1;
        uint threshold = num_parties / 2;
        uint num_images = atoi(argv[4]);
        uint batch_size = atoi(argv[5]);
        std::string model_path = (argv[6]);
        cout << model_path << endl;

        uint ring_size = 60;

        nNet = new NodeNetwork(nodeConfig, atoi(argv[1]), 1, ring_size, num_parties, threshold);
        minionn_main(model_path, num_parties, num_images, batch_size);

    } else if (!strcmp(argv[3], "q_mobilenet")) {
        if (argc < 10) {
            fprintf(stderr, "Incorrect input parameters for q_mobilenet\n");
            fprintf(stderr, "Usage: <id> <runtime-config> q_mobilenet <input_dim> <alpha_index> <batch_size> <num_iterations> <num_discarded> <model_path>\n");
            fprintf(stderr, "input_dim: 128, 160, 192, 224\n");
            fprintf(stderr, "alpha_index (give INDEX): 0.25, 0.5, 0.75, 1.0\n");
            delete nodeConfig;
            exit(1);
        }
        uint num_parties = numPeers + 1;
        uint threshold = num_parties / 2;
        uint input_dim = atoi(argv[4]);
        uint alpha_index = atoi(argv[5]);
        uint batch_size = atoi(argv[6]);
        uint num_iterations = atoi(argv[7]);
        uint num_discarded = atoi(argv[8]);
        std::string model_path = (argv[9]);
        cout << model_path << endl;

        uint ring_size = 30;
        cout << "input_dim = " << input_dim << endl;
        cout << "alpha_index = " << alpha_index << endl;
        cout << "batch_size = " << batch_size << endl;
        cout << "num_iterations = " << num_iterations << endl;
        cout << "num_discarded = " << num_discarded << endl;

        nNet = new NodeNetwork(nodeConfig, atoi(argv[1]), 1, ring_size, num_parties, threshold);
        q_mobilenet_main(model_path, num_parties, input_dim, alpha_index, batch_size, num_iterations, num_discarded);

    } else if (!strcmp(argv[3], "test")) {
        if (argc < 6) {
            fprintf(stderr, "Incorrect input parameters for testing\n");
            fprintf(stderr, "Usage: <id> <runtime-config> test <ring size(x bits)> <size of data(2^x)> <batch_size> \n");
            delete nodeConfig;
            exit(1);
        }
        uint num_parties = numPeers + 1;
        uint threshold = num_parties / 2;
        uint ring_size = atoi(argv[4]);
        uint size = atoi(argv[5]);
        uint batch_size = atoi(argv[6]);
        
        cout << "num_parties = " << num_parties << endl;
        cout << "threshold = " << threshold << endl;
        cout << "ring_size = " << ring_size << endl;
        cout << "size = " << size << endl;
        cout << "batch_size = " << batch_size << endl;

        nNet = new NodeNetwork(nodeConfig, atoi(argv[1]), 1, ring_size, num_parties, threshold);
        if (num_parties == 3) {
            test_protocols_3pc(nNet, nodeConfig, size, batch_size);
        }


        // if (num_parties == 7) {
        //    benchmark_mp_7(nNet, nodeConfig, size, batch_size);
        // }

        // benchmark_main(num_parties, argv[4], size, batch_size, num_iterations);
    }
    else if (!strcmp(argv[3], "eda_rb_pregen")) {
        if (argc < 10) {
            fprintf(stderr, "Incorrect input parameters for eda_rb_pregen\n");
            fprintf(stderr, "Usage: <id> <runtime-config> eda_rb_pregen <input_dim> <alpha_index> <batch_size> <num_iterations> <num_discarded> <path>\n");
            fprintf(stderr, "input_dim: 128, 160, 192, 224\n");
            fprintf(stderr, "alpha_index (give INDEX): 0.25, 0.5, 0.75, 1.0\n");
            delete nodeConfig;
            exit(1);
        }
        uint num_parties = numPeers + 1;
        uint threshold = num_parties / 2;
        uint input_dim = atoi(argv[4]);
        uint alpha_index = atoi(argv[5]);
        uint batch_size = atoi(argv[6]);
        uint num_iterations = atoi(argv[7]);
        uint num_discarded = atoi(argv[8]);
        std::string path = (argv[9]);
        cout << path << endl;
        uint ring_size = 30;

        cout << "input_dim = " << input_dim << endl;
        cout << "alpha_index = " << alpha_index << endl;
        cout << "batch_size = " << batch_size << endl;
        cout << "num_iterations = " << num_iterations << endl;
        cout << "num_discarded = " << num_discarded << endl;

        nNet = new NodeNetwork(nodeConfig, atoi(argv[1]), 1, ring_size, num_parties, threshold);

        eda_rb_pregen_main( num_parties, input_dim, alpha_index, batch_size, num_iterations, num_discarded, path);

    }

    else {
        fprintf(stderr, "invalid experiment name\n");
    }

    delete nodeConfig;

    return 0;
}

void benchmark_main(uint num_parties, char *protocol, uint size, uint batch_size, uint num_iterations) {

    if (num_parties == 3) {
        benchmark_3pc(nNet, nodeConfig, protocol, size, batch_size, num_iterations);
    } else if (num_parties == 5) {
        benchmark_5pc(nNet, nodeConfig, protocol, size, batch_size, num_iterations);
    } else if (num_parties == 7) {
        benchmark_7pc(nNet, nodeConfig, protocol, size, batch_size, num_iterations);
    } else {
        printf("ERROR: incorrect number of parties\n");
    }
}

void minionn_main(std::string model_path, uint num_parties, uint num_images, uint batch_size) {

    if (num_parties == 3) {
        minionn(nNet, nodeConfig, model_path, num_images, batch_size);
    } else if (num_parties == 5) {
        minionn_mp(nNet, nodeConfig, model_path, num_images, batch_size);
    } else {

        printf("ERROR: incorrect number of parties\n");
    }
}

void q_mobilenet_main(std::string mobile_path, uint num_parties, uint input_dim, uint alpha_index, uint batch_size, uint num_iterations, uint num_discarded) {

    if (num_parties == 3) {
        q_mobilenet(nNet, nodeConfig, mobile_path, num_iterations, batch_size, alpha_index, input_dim, num_discarded);
    } else if (num_parties == 5) {
        q_mobilenet_mp(nNet, nodeConfig, mobile_path, num_iterations, batch_size, alpha_index, input_dim, num_discarded);
    } else {

        printf("ERROR: incorrect number of parties\n");
    }
}

void eda_rb_pregen_main(uint num_parties, uint input_dim, uint alpha_index, uint batch_size, uint num_iterations, uint num_discarded, string path) {

    if (num_parties == 3) {
        eda_rb_pregen_3pc(nNet, nodeConfig,  num_iterations, batch_size, alpha_index, input_dim, num_discarded, path);
    } else if (num_parties == 5) {
        eda_rb_pregen_5pc(nNet, nodeConfig,  num_iterations, batch_size, alpha_index, input_dim, num_discarded, path);
    } else {

        printf("ERROR: incorrect number of parties\n");
    }
}
