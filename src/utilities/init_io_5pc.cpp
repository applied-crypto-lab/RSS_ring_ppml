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

#include "init_io_5pc.h"





void splitData_mp(Lint **output, long long *input, uint m, uint n, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    uint totalNumShares = nodeNet->getTotalNumShares();

    uint share_indeces[5][6] = {
        {4, 5, 6, 7, 8, 9}, // p1
        {7, 8, 1, 9, 2, 3}, // p2
        {9, 2, 5, 3, 6, 0}, // p3
        {3, 6, 8, 0, 1, 4}, // p4
        {0, 1, 2, 4, 5, 7}  // p5
    };

    Lint **splitInput = new Lint *[totalNumShares];
    for (i = 0; i < totalNumShares; i++) {
        splitInput[i] = new Lint[(m * n)];
        memset(splitInput[i], 0, sizeof(Lint) * (m * n));
    }

    for (i = 0; i < m * n; i++) {
        for (size_t j = 0; j < totalNumShares - 1; j++) {
            prg_aes_ni(splitInput[j] + i, key1, prg_key);
            splitInput[j][i] = splitInput[j][i] & nodeNet->SHIFT[ring_size];
        }
        splitInput[totalNumShares - 1][i] = (input[i] & ((Lint(1) << Lint(ring_size)) - Lint(1)));
        for (size_t j = 0; j < totalNumShares - 1; j++) {
            splitInput[totalNumShares - 1][i] -= splitInput[j][i];
        }
        splitInput[totalNumShares - 1][i] = splitInput[totalNumShares - 1][i] & nodeNet->SHIFT[ring_size];
    }

    for (size_t i = 0; i < numShares; i++) {
        output[i] = splitInput[share_indeces[pid - 1][i]];
    }
    free(prg_key);
}

void splitData_mp(Lint **output, vector<sLint> input, uint size, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    uint totalNumShares = nodeNet->getTotalNumShares();

    uint share_indeces[5][6] = {
        {4, 5, 6, 7, 8, 9}, // p1
        {7, 8, 1, 9, 2, 3}, // p2
        {9, 2, 5, 3, 6, 0}, // p3
        {3, 6, 8, 0, 1, 4}, // p4
        {0, 1, 2, 4, 5, 7}  // p5
    };

    Lint **splitInput = new Lint *[totalNumShares];
    for (i = 0; i < totalNumShares; i++) {
        splitInput[i] = new Lint[(size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size));
    }

    for (i = 0; i < size; i++) {
        for (size_t j = 0; j < totalNumShares - 1; j++) {
            prg_aes_ni(splitInput[j] + i, key1, prg_key);
            splitInput[j][i] = splitInput[j][i] & nodeNet->SHIFT[ring_size];
        }
        splitInput[totalNumShares - 1][i] = (input[i] & ((Lint(1) << Lint(ring_size)) - Lint(1)));
        for (size_t j = 0; j < totalNumShares - 1; j++) {
            splitInput[totalNumShares - 1][i] -= splitInput[j][i];
        }
        splitInput[totalNumShares - 1][i] = splitInput[totalNumShares - 1][i] & nodeNet->SHIFT[ring_size];
    }

    for (size_t i = 0; i < numShares; i++) {
        output[i] = splitInput[share_indeces[pid - 1][i]];
    }
    free(prg_key);
}

void splitData_mp(Lint **output, vector<Lint> input, uint size, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    uint totalNumShares = nodeNet->getTotalNumShares();

    uint share_indeces[5][6] = {
        {4, 5, 6, 7, 8, 9}, // p1
        {7, 8, 1, 9, 2, 3}, // p2
        {9, 2, 5, 3, 6, 0}, // p3
        {3, 6, 8, 0, 1, 4}, // p4
        {0, 1, 2, 4, 5, 7}  // p5
    };

    Lint **splitInput = new Lint *[totalNumShares];
    for (i = 0; i < totalNumShares; i++) {
        splitInput[i] = new Lint[(size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size));
    }

    for (i = 0; i < size; i++) {
        for (size_t j = 0; j < totalNumShares - 1; j++) {
            prg_aes_ni(splitInput[j] + i, key1, prg_key);
            splitInput[j][i] = splitInput[j][i] & nodeNet->SHIFT[ring_size];
        }
        splitInput[totalNumShares - 1][i] = (input[i] & ((Lint(1) << Lint(ring_size)) - Lint(1)));
        for (size_t j = 0; j < totalNumShares - 1; j++) {
            splitInput[totalNumShares - 1][i] -= splitInput[j][i];
        }
        splitInput[totalNumShares - 1][i] = splitInput[totalNumShares - 1][i] & nodeNet->SHIFT[ring_size];
    }

    for (size_t i = 0; i < numShares; i++) {
        output[i] = splitInput[share_indeces[pid - 1][i]];
    }
    free(prg_key);
}

void splitData_X_mp(Lint **output, vector<Lint> input, uint size, uint batch_size, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i, j;
    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    uint totalNumShares = nodeNet->getTotalNumShares();

    uint share_indeces[5][6] = {
        {4, 5, 6, 7, 8, 9}, // p1
        {7, 8, 1, 9, 2, 3}, // p2
        {9, 2, 5, 3, 6, 0}, // p3
        {3, 6, 8, 0, 1, 4}, // p4
        {0, 1, 2, 4, 5, 7}  // p5
    };

    Lint **splitInput = new Lint *[totalNumShares];
    for (i = 0; i < totalNumShares; i++) {
        splitInput[i] = new Lint[(size * batch_size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size * batch_size));
    }

    for (j = 0; j < batch_size; j++) {

        for (i = 0; i < size; i++) {
            for (size_t k = 0; k < totalNumShares - 1; k++) {
                prg_aes_ni(splitInput[k] + j * size + i, key1, prg_key);
                splitInput[k][j * size + i] = splitInput[k][j * size + i] & nodeNet->SHIFT[ring_size];
            }
            splitInput[totalNumShares - 1][i] = (input[i] & ((Lint(1) << Lint(ring_size)) - Lint(1)));
            for (size_t k = 0; k < totalNumShares - 1; k++) {
                splitInput[totalNumShares - 1][j * size + i] -= splitInput[k][j * size + i];
            }
            splitInput[totalNumShares - 1][j * size + i] = splitInput[totalNumShares - 1][j * size + i] & nodeNet->SHIFT[ring_size];
        }
    }
    for (size_t i = 0; i < numShares; i++) {
        output[i] = splitInput[share_indeces[pid - 1][i]];
    }

    free(prg_key);
}


void splitData_mp(Lint **output, vector<quant_param> input, uint size, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    uint totalNumShares = nodeNet->getTotalNumShares();

    uint share_indeces[5][6] = {
        {4, 5, 6, 7, 8, 9}, // p1
        {7, 8, 1, 9, 2, 3}, // p2
        {9, 2, 5, 3, 6, 0}, // p3
        {3, 6, 8, 0, 1, 4}, // p4
        {0, 1, 2, 4, 5, 7}  // p5
    };

    Lint **splitInput = new Lint *[totalNumShares];
    for (i = 0; i < totalNumShares; i++) {
        splitInput[i] = new Lint[(size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size));
    }

    for (i = 0; i < size; i++) {
        for (size_t j = 0; j < totalNumShares - 1; j++) {
            prg_aes_ni(splitInput[j] + i, key1, prg_key);
            splitInput[j][i] = splitInput[j][i] & nodeNet->SHIFT[ring_size];
        }
        splitInput[totalNumShares - 1][i] = (input[i].zero_point & ((Lint(1) << Lint(ring_size)) - Lint(1)));
        for (size_t j = 0; j < totalNumShares - 1; j++) {
            splitInput[totalNumShares - 1][i] -= splitInput[j][i];
        }
        splitInput[totalNumShares - 1][i] = splitInput[totalNumShares - 1][i] & nodeNet->SHIFT[ring_size];
    }

    for (size_t i = 0; i < numShares; i++) {
        output[i] = splitInput[share_indeces[pid - 1][i]];
    }
    free(prg_key);
}

void generateData_mp(Lint **output, uint size, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    uint totalNumShares = nodeNet->getTotalNumShares();

    uint share_indeces[5][6] = {
        {4, 5, 6, 7, 8, 9}, // p1
        {7, 8, 1, 9, 2, 3}, // p2
        {9, 2, 5, 3, 6, 0}, // p3
        {3, 6, 8, 0, 1, 4}, // p4
        {0, 1, 2, 4, 5, 7}  // p5
    };

    Lint **splitInput = new Lint *[totalNumShares];
    for (i = 0; i < totalNumShares; i++) {
        splitInput[i] = new Lint[(size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size));
    }

    for (i = 0; i < size; i++) {
        for (size_t j = 0; j < totalNumShares - 1; j++) {
            prg_aes_ni(splitInput[j] + i, key1, prg_key);
            splitInput[j][i] = splitInput[j][i] & nodeNet->SHIFT[ring_size];
        }
        splitInput[totalNumShares - 1][i] = (Lint(i) & ((Lint(1) << Lint(ring_size)) - Lint(1)));
        for (size_t j = 0; j < totalNumShares - 1; j++) {
            splitInput[totalNumShares - 1][i] -= splitInput[j][i];
        }
        splitInput[totalNumShares - 1][i] = splitInput[totalNumShares - 1][i] & nodeNet->SHIFT[ring_size];
    }

    for (size_t i = 0; i < numShares; i++) {
        output[i] = splitInput[share_indeces[pid - 1][i]];
    }
    free(prg_key);
}

void splitData_X_mp(Lint **output, long long *input, uint size, uint batch_size, uint tracker, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    uint totalNumShares = nodeNet->getTotalNumShares();

    uint share_indeces[5][6] = {
        {4, 5, 6, 7, 8, 9}, // p1
        {7, 8, 1, 9, 2, 3}, // p2
        {9, 2, 5, 3, 6, 0}, // p3
        {3, 6, 8, 0, 1, 4}, // p4
        {0, 1, 2, 4, 5, 7}  // p5
    };

    Lint **splitInput = new Lint *[totalNumShares];
    for (i = 0; i < totalNumShares; i++) {
        splitInput[i] = new Lint[(size * batch_size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size * batch_size));
    }

    for (i = 0; i < size * batch_size; i++) {
        for (size_t k = 0; k < totalNumShares - 1; k++) {
            prg_aes_ni(splitInput[k] + i, key1, prg_key);
            splitInput[k][i] = splitInput[k][i] & nodeNet->SHIFT[ring_size];
        }
        splitInput[totalNumShares - 1][i] = (input[tracker * size * batch_size + i] & ((Lint(1) << Lint(ring_size)) - Lint(1)));
        for (size_t k = 0; k < totalNumShares - 1; k++) {
            splitInput[totalNumShares - 1][i] -= splitInput[k][i];
        }
        splitInput[totalNumShares - 1][i] = splitInput[totalNumShares - 1][i] & nodeNet->SHIFT[ring_size];
    }
    for (size_t i = 0; i < numShares; i++) {
        output[i] = splitInput[share_indeces[pid - 1][i]];
    }
    free(prg_key);
}

void q_fullSetup_mp(
    Lint **x,
    Lint ***weights,
    Lint ***biases,
    Lint **relu_bounds,
    Lint **trunc_vals,
    Lint **input_zp,   // (z_1)
    Lint **weights_zp, // (z_2)
    Lint **output_zp,  // (z_3)
    string model_path,
    string image_path,
    vector<string> all_names,
    vector<vector<int>> model_shapes,
    vector<vector<int>> bias_dim,
    double alpha,
    uint input_dim,
    Lint public_M,
    uint ring_size,
    uint batch_size,
    NodeNetwork *nodeNet) {

    uint num_layers = model_shapes.size();

    string model_path_csv = model_path + "csv/";
    // cout<<"model_path_csv : "<<model_path_csv<<endl;;

    vector<vector<sLint>> in_weights;
    vector<vector<sLint>> in_biases;

    vector<sLint> in_relu_bounds;

    vector<quant_param> w_quant_params;
    vector<quant_param> b_quant_params;
    vector<quant_param> output_quant_params;

    vector<Lint> in_x = quant_read_matrix(image_path + input_image.at(test_im) + ".csv");

    vector<sLint> v_relu_bounds;

    vector<string> weight_names = read_fnames(all_names.at(0));
    vector<string> weight_param_names = read_fnames(all_names.at(1)); // m_2
    vector<string> bias_names = read_fnames(all_names.at(2));
    vector<string> bias_param_names = read_fnames(all_names.at(3));
    vector<string> output_param_names = read_fnames(all_names.at(4)); // m_3

    // cout<<"model_shapes.size() : "<<model_shapes.size()<<endl;
    // cout<<"weight_names.size() : "<<weight_names.size()<<endl;
    // cout<<"bias_names.size() : "<<bias_names.size()<<endl;

    for (size_t i = 0; i < model_shapes.size(); i++) {
        // cout<<weight_names.at(i)<<endl;
        in_weights.push_back(s_quant_reorderWeights(
            quant_read_matrix(model_path_csv + weight_names.at(i)), model_shapes.at(i).at(2),
            model_shapes.at(i).at(3),
            model_shapes.at(i).at(0),
            float(alpha)));
        in_biases.push_back(s_quant_read_matrix(model_path_csv + bias_names.at(i)));
        // cout<<"in_weights.at(i).size() :  "<<in_weights.at(i).size()<<endl;
    }
    // cout<<"in_weights.size() : "<<in_weights.size()<<endl;
    // cout<<"in_biases.size() : "<<in_biases.size()<<endl;

    for (size_t i = 0; i < model_shapes.size(); i++) {

        w_quant_params.push_back(read_quant_params(model_path_csv + weight_param_names.at(i)));
        b_quant_params.push_back(read_quant_params(model_path_csv + bias_param_names.at(i)));
        output_quant_params.push_back(read_quant_params(model_path_csv + output_param_names.at(i)));
    }
    // cout<<"output_quant_params.size() : "<<output_quant_params.size()<<endl;
    // cout<<"w_quant_params.size() : "<<w_quant_params.size()<<endl;
    // cout<<"b_quant_params.size() : "<<b_quant_params.size()<<endl;

    vector<quant_param> input_quant_params;
    input_quant_params.push_back(read_quant_params(model_path_csv + "input_quant_params.csv"));

    for (size_t i = 0; i < model_shapes.size() - 1; i++) {
        input_quant_params.push_back(quant_param(output_quant_params.at(i).scale, 0));
    }
    // cout<<"input_quant_params.size() : "<<input_quant_params.size()<<endl;

    vector<double> deltas;
    deltas.push_back(1.0); // first layer is always 1
    vector<Lint> num_bits;
    vector<Lint> pre_trunc_vals;
    for (size_t i = 0; i < model_shapes.size(); i++) {
        num_bits.push_back(floor(log2(deltas.at(i) * 6.0 / (255 * input_quant_params.at(i).scale * w_quant_params.at(i).scale))));

        pre_trunc_vals.push_back(1 << (public_M - num_bits.at(i))); // computing 2^{M-m} values

        deltas.push_back(deltas.at(i) * 6.0 / ((1 << num_bits.at(i)) * 255 * input_quant_params.at(i).scale * w_quant_params.at(i).scale));
        // cout<<i<<" : "<<num_bits.at(i)<<"\t "<<deltas.at(i)<<"\t "<< deltas.at(i)*6.0/(input_quant_params.at(i).scale * w_quant_params.at(i).scale)<<"\t "<<pre_trunc_vals.at(i)<<endl;
    }

    // scaling the zero point in the last layer, so we can use the same q_conv2d function as the first layer

    // print_params(w_quant_params.at(num_layers - 1), b_quant_params.at(num_layers - 1), input_quant_params.at(num_layers - 1), output_quant_params.at(num_layers - 1));

    // cout<<"out_z_3 : " << output_quant_params.at(num_layers - 1).zero_point << endl;

    output_quant_params.at(num_layers - 1).zero_point = output_quant_params.at(num_layers - 1).scale * deltas.at(num_layers - 1) * output_quant_params.at(num_layers - 1).zero_point / (input_quant_params.at(num_layers - 1).scale * w_quant_params.at(num_layers - 1).scale);

    // print_params(w_quant_params.at(num_layers - 1), b_quant_params.at(num_layers - 1), input_quant_params.at(num_layers - 1), output_quant_params.at(num_layers - 1));

    // applying the deltas to the biases
    for (size_t i = 1; i < in_biases.size(); i++) {
        applyScale(deltas.at(i), in_biases.at(i));
    }

    for (size_t i = 0; i < model_shapes.size(); i++) {

        // computing all the relu bounds for the network
        in_relu_bounds.push_back((deltas.at(i) * 6.0) / (input_quant_params.at(i).scale * w_quant_params.at(i).scale));
    }

    in_relu_bounds.at(num_layers - 1) = (255 * deltas.at(num_layers - 1) * output_quant_params.at(num_layers - 1).scale) / (input_quant_params.at(num_layers - 1).scale * w_quant_params.at(num_layers - 1).scale);

    for (size_t i = 0; i < num_layers; i++) {
        uint weight_size = model_shapes.at(i).at(0) * model_shapes.at(i).at(1) * model_shapes.at(i).at(2) * model_shapes.at(i).at(3);
        // cout<<"weight_size :  "<<weight_size<<endl;
        // cout<<"in_weights.at(i).size() :  "<<in_weights.at(i).size()<<endl;
        // cout<<"bias_dim.at(i).at(0) :  "<<bias_dim.at(i).at(0)<<endl;
        // cout<<"in_biases.at(i).size() :  "<<in_biases.at(i).size()<<endl;
        splitData_mp(weights[i], in_weights.at(i), weight_size, ring_size, nodeNet);
        splitData_mp(biases[i], in_biases.at(i), bias_dim.at(i).at(0), ring_size, nodeNet);
    }

    splitData_mp(input_zp, input_quant_params, num_layers, ring_size, nodeNet);
    splitData_mp(weights_zp, w_quant_params, num_layers, ring_size, nodeNet);
    splitData_mp(output_zp, output_quant_params, num_layers, ring_size, nodeNet);

    splitData_mp(relu_bounds, in_relu_bounds, num_layers, ring_size, nodeNet);
    splitData_mp(trunc_vals, pre_trunc_vals, num_layers, ring_size, nodeNet);
    splitData_X_mp(x, in_x, input_dim * input_dim * 3, batch_size, ring_size, nodeNet);
}