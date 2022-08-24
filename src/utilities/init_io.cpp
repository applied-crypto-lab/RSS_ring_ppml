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

#include "init_io.h"

void splitData(Lint **output, long long *input, uint m, uint n, uint ring_size, NodeNetwork *nodeNet) {
    // key stuff
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();

    Lint **splitInput = new Lint *[3];
    for (i = 0; i < 3; i++) {
        splitInput[i] = new Lint[(m * n)];
        memset(splitInput[i], 0, sizeof(Lint) * (m * n));
    }

    for (i = 0; i < m * n; i++) {
        prg_aes_ni(splitInput[0] + i, key1, prg_key);
        prg_aes_ni(splitInput[1] + i, key1, prg_key);

        splitInput[0][i] = splitInput[0][i] & nodeNet->SHIFT[ring_size];
        splitInput[1][i] = splitInput[1][i] & nodeNet->SHIFT[ring_size];
        splitInput[2][i] = ((input[i] & ((Lint(1) << Lint(ring_size)) - Lint(1))) - splitInput[0][i] - splitInput[1][i]) & nodeNet->SHIFT[ring_size];
    }

    switch (pid) {
    case 1:
        memcpy(output[0], splitInput[1], sizeof(Lint) * (m * n));
        memcpy(output[1], splitInput[2], sizeof(Lint) * (m * n));
        break;
    case 2:
        memcpy(output[0], splitInput[2], sizeof(Lint) * (m * n));
        memcpy(output[1], splitInput[0], sizeof(Lint) * (m * n));
        break;
    case 3:
        memcpy(output[0], splitInput[0], sizeof(Lint) * (m * n));
        memcpy(output[1], splitInput[1], sizeof(Lint) * (m * n));
        break;
    }

    for (size_t i = 0; i < 3; i++) {
        delete[] splitInput[i];
    }
    delete[] splitInput;

    free(prg_key);
}


void splitData(Lint **output, vector<sLint> input, uint size, uint ring_size, NodeNetwork *nodeNet) {

    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();

    Lint **splitInput = new Lint *[3];
    for (i = 0; i < 3; i++) {
        splitInput[i] = new Lint[(size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size));
    }

    for (i = 0; i < size; i++) {
        prg_aes_ni(splitInput[0] + i, key1, prg_key);
        prg_aes_ni(splitInput[1] + i, key1, prg_key);

        splitInput[0][i] = splitInput[0][i] & nodeNet->SHIFT[ring_size];
        splitInput[1][i] = splitInput[1][i] & nodeNet->SHIFT[ring_size];
        // memset(splitInput[0], 0, sizeof(Lint)*(m*n)); //USED FOR TESTING
        // memset(splitInput[1], 0, sizeof(Lint)*(m*n)); //USED FOR TESTING

        // CHECK HERE IF THIS IS STILL WHAT WE NEED TO DO
        splitInput[2][i] = ((input[i] & ((Lint(1) << Lint(ring_size)) - Lint(1))) - splitInput[0][i] - splitInput[1][i]) & nodeNet->SHIFT[ring_size];
    }

    switch (pid) {
    case 1:
        memcpy(output[0], splitInput[1], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[2], sizeof(Lint) * (size));
        break;
    case 2:
        memcpy(output[0], splitInput[2], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[0], sizeof(Lint) * (size));
        break;
    case 3:
        memcpy(output[0], splitInput[0], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[1], sizeof(Lint) * (size));
        break;
    }

    for (size_t i = 0; i < 3; i++) {
        delete[] splitInput[i];
    }
    delete[] splitInput;

    free(prg_key);
}

void splitData(Lint **output, vector<Lint> input, uint size, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();

    Lint **splitInput = new Lint *[3];
    for (i = 0; i < 3; i++) {
        splitInput[i] = new Lint[(size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size));
    }

    for (i = 0; i < size; i++) {
        prg_aes_ni(splitInput[0] + i, key1, prg_key);
        prg_aes_ni(splitInput[1] + i, key1, prg_key);

        splitInput[0][i] = splitInput[0][i] & nodeNet->SHIFT[ring_size];
        splitInput[1][i] = splitInput[1][i] & nodeNet->SHIFT[ring_size];
        // memset(splitInput[0], 0, sizeof(Lint)*(m*n)); //USED FOR TESTING
        // memset(splitInput[1], 0, sizeof(Lint)*(m*n)); //USED FOR TESTING

        // CHECK HERE IF THIS IS STILL WHAT WE NEED TO DO
        splitInput[2][i] = ((input[i] & ((Lint(1) << Lint(ring_size)) - Lint(1))) - splitInput[0][i] - splitInput[1][i]) & nodeNet->SHIFT[ring_size];
    }

    switch (pid) {
    case 1:
        memcpy(output[0], splitInput[1], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[2], sizeof(Lint) * (size));
        break;
    case 2:
        memcpy(output[0], splitInput[2], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[0], sizeof(Lint) * (size));
        break;
    case 3:
        memcpy(output[0], splitInput[0], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[1], sizeof(Lint) * (size));
        break;
    }

    for (size_t i = 0; i < 3; i++) {
        delete[] splitInput[i];
    }
    delete[] splitInput;

    free(prg_key);
}



void splitData_X(Lint **output, vector<Lint> input, uint size, uint batch_size, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i, j;
    int pid = nodeNet->getID();

    Lint **splitInput = new Lint *[3];
    for (i = 0; i < 3; i++) {
        splitInput[i] = new Lint[(size * batch_size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size * batch_size));
    }

    for (j = 0; j < batch_size; j++) {
        for (i = 0; i < size; i++) {
            prg_aes_ni(splitInput[0] + j * size + i, key1, prg_key);
            prg_aes_ni(splitInput[1] + j * size + i, key1, prg_key);

            splitInput[0][j * size + i] = splitInput[0][j * size + i] & nodeNet->SHIFT[ring_size];
            // splitInput[1][j * size + i] = splitInput[1][i] & nodeNet->SHIFT[ring_size];
            splitInput[1][j * size + i] = splitInput[1][j * size + i] & nodeNet->SHIFT[ring_size]; // CHECK IF THIS IS RIGHT, IT WAS ORIGINALLY ABOVE

            splitInput[2][j * size + i] = ((input[i] & ((Lint(1) << Lint(ring_size)) - Lint(1))) - splitInput[0][j * size + i] - splitInput[1][j * size + i]) & nodeNet->SHIFT[ring_size];
        }
    }

    switch (pid) {
    case 1:
        memcpy(output[0], splitInput[1], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[2], sizeof(Lint) * (size));
        break;
    case 2:
        memcpy(output[0], splitInput[2], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[0], sizeof(Lint) * (size));
        break;
    case 3:
        memcpy(output[0], splitInput[0], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[1], sizeof(Lint) * (size));
        break;
    }

    for (size_t i = 0; i < 3; i++) {
        delete[] splitInput[i];
    }
    delete[] splitInput;

    free(prg_key);
}

void splitData(Lint **output, vector<quant_param> input, uint size, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();

    Lint **splitInput = new Lint *[3];
    for (i = 0; i < 3; i++) {
        splitInput[i] = new Lint[(size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size));
    }

    for (i = 0; i < size; i++) {
        prg_aes_ni(splitInput[0] + i, key1, prg_key);
        prg_aes_ni(splitInput[1] + i, key1, prg_key);

        splitInput[0][i] = splitInput[0][i] & nodeNet->SHIFT[ring_size];
        splitInput[1][i] = splitInput[1][i] & nodeNet->SHIFT[ring_size];
        // memset(splitInput[0], 0, sizeof(Lint)*(m*n)); //USED FOR TESTING
        // memset(splitInput[1], 0, sizeof(Lint)*(m*n)); //USED FOR TESTING

        // CHECK HERE IF THIS IS STILL WHAT WE NEED TO DO
        splitInput[2][i] = ((input[i].zero_point & ((Lint(1) << Lint(ring_size)) - Lint(1))) - splitInput[0][i] - splitInput[1][i]) & nodeNet->SHIFT[ring_size];
    }

    switch (pid) {
    case 1:
        memcpy(output[0], splitInput[1], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[2], sizeof(Lint) * (size));
        break;
    case 2:
        memcpy(output[0], splitInput[2], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[0], sizeof(Lint) * (size));
        break;
    case 3:
        memcpy(output[0], splitInput[0], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[1], sizeof(Lint) * (size));
        break;
    }

    for (size_t i = 0; i < 3; i++) {
        delete[] splitInput[i];
    }
    delete[] splitInput;

    free(prg_key);
}



// used to randomly generate data
void generateData(Lint **output, uint size, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();

    Lint **splitInput = new Lint *[3];
    for (i = 0; i < 3; i++) {
        splitInput[i] = new Lint[(size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size));
    }

    for (i = 0; i < size; i++) {
        prg_aes_ni(splitInput[0] + i, key1, prg_key);
        prg_aes_ni(splitInput[1] + i, key1, prg_key);

        splitInput[0][i] = splitInput[0][i] & nodeNet->SHIFT[ring_size];
        splitInput[1][i] = splitInput[1][i] & nodeNet->SHIFT[ring_size];
        splitInput[2][i] = ((Lint(i) & ((Lint(1) << Lint(ring_size)) - Lint(1))) - splitInput[0][i] - splitInput[1][i]) & nodeNet->SHIFT[ring_size];
    }

    switch (pid) {
    case 1:
        memcpy(output[0], splitInput[1], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[2], sizeof(Lint) * (size));
        break;
    case 2:
        memcpy(output[0], splitInput[2], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[0], sizeof(Lint) * (size));
        break;
    case 3:
        memcpy(output[0], splitInput[0], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[1], sizeof(Lint) * (size));
        break;
    }

    for (size_t i = 0; i < 3; i++) {
        delete[] splitInput[i];
    }
    delete[] splitInput;

    free(prg_key);
}


void readFile(long long *output, uint rows, uint cols, Lint fractional, std::string path) {

    uint i = 0, j = 0, k = 0, len, last = 0;
    std::ifstream file(path.c_str());
    std::string num = "";
    std::string line;
    if (file.is_open()) {
        while (getline(file, line)) {

            k = 0, last = 0, j = 0;
            len = line.length();

            while (k != len) {
                if (line[k] == ',' || k == len - 1) {

                    // for converting string into number
                    //  output[i*cols+j] = roundf(atof(num.append(line,last,k).c_str()))*fractional;
                    output[i * cols + j] = roundf((atof(num.append(line, last, k).c_str())) * fractional);
                    // output[i*cols+j] = (long long )(atof(num.append(line,last,k).c_str())*fractional);
                    // printf("%f\n", output[i][j]);

                    // Emtying string for getting next data
                    num = "";
                    // increasing column number after saving data
                    j++;

                    if (k != len - 1)
                        last = k + 1;
                }
                k++;
            }
            // increase row number for array
            i++;
        }
        file.close();
    } else
        printf("Input file error!\n");
}

void splitX(Lint **output, long long *input, uint size, uint batch_size, uint tracker, uint ring_size, NodeNetwork *nodeNet) {
    uint8_t raw_key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    __m128i *prg_key = offline_prg_keyschedule(raw_key);
    // setup prg seed(k1, k2, k3)
    uint8_t key1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    uint i;
    int pid = nodeNet->getID();

    Lint **splitInput = new Lint *[3];
    for (i = 0; i < 3; i++) {
        splitInput[i] = new Lint[(size * batch_size)];
        memset(splitInput[i], 0, sizeof(Lint) * (size * batch_size));
    }

    // for (i = 0; i < size; i++) {
    //     // printf("res[%i] : %llu \n", i, res[i] ); //& nodeNet->SHIFT[48]);
    //     printf("input[%i] : %llu \n", i, input[tracker*size*batch_size + i] ); //& nodeNet->SHIFT[48]);
    //     // print_binary(res[i], final_ring_size);
    // }
    for (i = 0; i < size * batch_size; i++) {
        prg_aes_ni(splitInput[0] + i, key1, prg_key);
        prg_aes_ni(splitInput[1] + i, key1, prg_key);

        splitInput[0][i] = splitInput[0][i] & nodeNet->SHIFT[ring_size];
        splitInput[1][i] = splitInput[1][i] & nodeNet->SHIFT[ring_size];
        // memset(splitInput[0], 0, sizeof(Lint)*(m*n)); //USED FOR TESTING
        // memset(splitInput[1], 0, sizeof(Lint)*(m*n)); //USED FOR TESTING

        splitInput[2][i] = ((input[tracker * size * batch_size + i] & ((Lint(1) << Lint(ring_size)) - Lint(1))) - splitInput[0][i] - splitInput[1][i]) & nodeNet->SHIFT[ring_size];
        // splitInput[2][i] = ((input[i] & ((Lint(1) << Lint(ring_size)) - Lint(1))) - splitInput[0][i] - splitInput[1][i]) & nodeNet->SHIFT[ring_size];

        // splitInput[2][i] = ((input[i] & ((Lint(1) << Lint(ring_size)) - Lint(1))) - splitInput[0][i] - splitInput[1][i]);

        // splitInput[2][i] = (input[i]  - splitInput[0][i] - splitInput[1][i]) & nodeNet->SHIFT[ring_size];
    }

    switch (pid) {
    case 1:
        memcpy(output[0], splitInput[1], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[2], sizeof(Lint) * (size));
        break;
    case 2:
        memcpy(output[0], splitInput[2], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[0], sizeof(Lint) * (size));
        break;
    case 3:
        memcpy(output[0], splitInput[0], sizeof(Lint) * (size));
        memcpy(output[1], splitInput[1], sizeof(Lint) * (size));
        break;
    }

    for (size_t i = 0; i < 3; i++) {
        delete[] splitInput[i];
    }
    delete[] splitInput;

    free(prg_key);
}



void readX(long long *output, uint rows, uint cols, std::string path) {
    // void readX(long long  **output, uint rows, uint cols, Lint fractional, std::string path){

    uint i = 0, j = 0, k = 0, len, last = 0;
    std::ifstream file(path.c_str());
    std::string num = "";
    std::string line;
    if (file.is_open()) {
        while (getline(file, line)) {

            k = 0, last = 0, j = 0;
            len = line.length();

            while (k != len) {
                if (line[k] == ',' || k == len - 1) {

                    // for converting string into number
                    //  output[i][j] = roundf(atof(num.append(line,last,k).c_str()))*fractional;
                    output[i * cols + j] = roundf(atof(num.append(line, last, k).c_str()));
                    // output[i*cols+j] = (long long )(atof(num.append(line,last,k).c_str())*fractional);
                    // printf("%f\n", output[i][j]);

                    // Emtying string for getting next data
                    num = "";
                    // increasing column number after saving data
                    j++;

                    if (k != len - 1)
                        last = k + 1;
                }
                k++;
            }
            // increase row number for array
            i++;
        }
        file.close();
    } else
        printf("Input file error!\n");
}

void readB(long long *output, uint rows, uint cols, Lint fractional, std::string path, uint n) {

    uint i = 0, j = 0, k = 0, len, last = 0;
    std::ifstream file(path.c_str());
    std::string num = "";
    std::string line;
    if (file.is_open()) {
        while (getline(file, line)) {

            k = 0, last = 0, j = 0;
            len = line.length();

            while (k != len) {
                if (line[k] == ',' || k == len - 1) {

                    // for converting string into number
                    //  output[i*cols+j] = roundf(atof(num.append(line,last,k).c_str()))*fractional;
                    output[i * cols + j] = roundf((atof(num.append(line, last, k).c_str())) * pow(fractional, n));
                    // output[i*cols+j] = (long long )(atof(num.append(line,last,k).c_str())*fractional);
                    // printf("%f\n", output[i][j]);

                    // Emtying string for getting next data
                    num = "";
                    // increasing column number after saving data
                    j++;

                    if (k != len - 1)
                        last = k + 1;
                }
                k++;
            }
            // increase row number for array
            i++;
        }
        file.close();
    } else
        printf("Input file error!\n");
}

vector<double> read_matrix(string path) {

    // ifstream file;

    ifstream file(path);
    // file.open(path.c_str());
    string line;
    vector<double> matrix;
    while (file.good()) {
        getline(file, line);
        if (!line.empty()) {
            // cout << stod(line) << endl;
            matrix.push_back(stod(line));
        }
    }
    file.close();

    return matrix;
}

vector<Lint> quant_read_matrix(string path) {

    ifstream file(path);
    string line;
    vector<Lint> matrix;

    while (file.good()) {
        getline(file, line);
        if (!line.empty()) {
            // cout << stod(line) << endl;
            matrix.push_back(stod(line));
        }
    }
    file.close();

    return matrix;
}

vector<uint> uint_read_matrix(string path) {

    ifstream file(path);
    string line;
    vector<uint> matrix;

    while (file.good()) {
        getline(file, line);
        if (!line.empty()) {
            // cout << stod(line) << endl;
            matrix.push_back(stod(line));
        }
    }
    file.close();

    return matrix;
}

vector<sLint> s_quant_read_matrix(string path) {
    ifstream file(path);
    string line;
    vector<sLint> matrix;
    while (file.good()) {
        getline(file, line);
        if (!line.empty()) {
            matrix.push_back(stod(line));
        }
    }
    file.close();
    return matrix;
}

vector<string> read_fnames(string path) {

    ifstream file(path);
    string line;
    vector<string> fnames;
    while (file.good()) {
        getline(file, line, '\n');
        if (!line.empty()) {
            fnames.push_back(line);
        }
    }

    return fnames;
}

vector<string> read_fnames_with_mobile_path(string mobile_path,string path) {

    ifstream file(path);
    string line;
    vector<string> fnames;
    while (file.good()) {
        getline(file, line, '\n');
        if (!line.empty()) {
            fnames.push_back(mobile_path + line);
        }
    }

    return fnames;
}


quant_param read_quant_params(string path) {
    ifstream file(path);
    string line;
    struct quant_param params;
    while (file.good()) {
        getline(file, line);
        if (!line.empty()) {
            params.scale = stod(line);
        }
        getline(file, line);
        if (!line.empty()) {

            params.zero_point = stoi(line);
        }
    }
    file.close();
    return params;
}

void write_layer(vector<double> x, int layer_ctr, string output_path, string input_image) {

    // mkdir((input_image + "_" + output_path).c_str(), 0755);

    // ofstream f(input_image + "_" + output_path + "x_" + to_string(layer_ctr - 1) + ".csv");

    // for (int k = 0; k < x.size(); k++) {
    //     // f << fixed;
    //     // f << setprecision(15);
    //     f << x.at(k) << "\n";
    // }

    // f.close();
}

vector<vector<int>> read_model_shape(string path) {

    ifstream file(path);
    string line, word;
    vector<vector<int>> model_shape;
    while (file.good()) {

        getline(file, line, '\n');
        // cout << line << endl;
        stringstream ss(line);

        vector<int> row;

        if (!line.empty()) {
            while (getline(ss, word, ',')) {
                row.push_back(stoi(word));
            }
        }
        model_shape.push_back(row);
    }

    return model_shape;
}

void print_params(quant_param w_quant_params, quant_param b_quant_params, quant_param input_quant_params, quant_param output_quant_params) {
    cout << "W:   (" << w_quant_params.scale << ", " << w_quant_params.zero_point << ")" << endl;
    cout << "b:   (" << b_quant_params.scale << ", " << b_quant_params.zero_point << ")" << endl;
    // cout<<"b_c: ("<<w_quant_params.scale*input_quant_params.scale <<", "<<b_quant_params.zero_point<<")" << endl;
    cout << "in:  (" << input_quant_params.scale << ", " << input_quant_params.zero_point << ")" << endl;
    cout << "out: (" << output_quant_params.scale << ", " << output_quant_params.zero_point << ")" << endl;
}

// used to get final model shapes based off of alpha
void scaleModel(vector<vector<int>> &model_shapes, double alpha) {
    for (size_t i = 0; i < model_shapes.size(); i++) {
        // input channel
        if (model_shapes.at(i).at(2) == 3) {
            // no change
            model_shapes.at(i).at(2) = model_shapes.at(i).at(2);
        } else {
            model_shapes.at(i).at(2) = model_shapes.at(i).at(2) * alpha;
        }
        // output channel
        if (model_shapes.at(i).at(3) == 1001 || model_shapes.at(i).at(3) == 1) {
            // no change
            model_shapes.at(i).at(3) = model_shapes.at(i).at(3);
        } else {
            model_shapes.at(i).at(3) = model_shapes.at(i).at(3) * alpha;
        }
    }
}

// used to get final model shapes based off of alpha
void scaleBiases(vector<vector<int>> &bias_shapes, double alpha) {
    for (size_t i = 0; i < bias_shapes.size(); i++) {
        // output channel
        if (bias_shapes.at(i).at(0) == 1001) {
            // no change
            bias_shapes.at(i).at(0) = bias_shapes.at(i).at(0);
        } else {
            bias_shapes.at(i).at(0) = bias_shapes.at(i).at(0) * alpha;
        }
    }
}

void scaleIntermediateLayers(vector<vector<int>> &int_layers, double alpha) {

    for (size_t i = 0; i < int_layers.size(); i++) {
        // output channel
        if (int_layers.at(i).at(3) == 1001) {
            // no change
            int_layers.at(i).at(3) = int_layers.at(i).at(3);
        } else {
            int_layers.at(i).at(3) = int_layers.at(i).at(3) * alpha;
        }
    }
}

vector<sLint> q_applyScale(double scale, vector<double> &x) {
    vector<sLint> y;
    for (size_t i = 0; i < x.size(); i++) {
        // y.at(i) = scale * x.at(i);
        y.push_back(scale * x.at(i));
    }
    return y;
}

void applyScale(double scale, vector<sLint> &x) {
    for (size_t i = 0; i < x.size(); i++) {
        x.at(i) = scale * x.at(i);
    }
}

vector<double> reorderWeights(vector<double> weights, int in_channels, int out_channels, int kernel) {

    vector<double> newWeights;

    for (int j = 0; j < (in_channels)*kernel * kernel; j++) {
        for (int i = 0; i < (out_channels); i++) {
            newWeights.push_back(weights[i * (kernel * kernel * (in_channels)) + j]);
        }
    }
    return newWeights;
}

vector<Lint> quant_reorderWeights(vector<Lint> weights, int in_channels, int out_channels, int kernel, float alpha) {

    vector<Lint> newWeights;
    int updated_in, updated_out;

    updated_out = out_channels * alpha;
    // cout << "out_channels : " << out_channels << "\t";
    // cout << "in_channels : " << in_channels << endl;

    // needed for the input layer
    if (in_channels == 3) {
        updated_in = in_channels;
    } else {
        updated_in = in_channels * alpha;
    }
    // needed for the output layer

    if (out_channels == 1001 || out_channels == 1) {
        updated_out = out_channels;
    } else {
        updated_out = out_channels * alpha;
    }
    // cout << "updated_out : " << updated_out<< "\t";
    // cout << "updated_in : " << updated_in<<endl;

    for (int j = 0; j < (updated_in)*kernel * kernel; j++) {
        for (int i = 0; i < (updated_out); i++) {
            newWeights.push_back(weights[i * (kernel * kernel * (updated_in)) + j]);
        }
    }
    return newWeights;
}

vector<sLint> s_quant_reorderWeights(vector<Lint> weights, int in_channels, int out_channels, int kernel, float alpha) {
    // cout<<"expected.size() :  "<<weights.size()<<endl;

    vector<sLint> newWeights;
    // int updated_in, updated_out;

    for (int j = 0; j < (in_channels)*kernel * kernel; j++) {
        for (int i = 0; i < (out_channels); i++) {
            newWeights.push_back(weights[i * (kernel * kernel * (in_channels)) + j]);
        }
    }
    return newWeights;
}

// calculates the alphas and betas, applies them to the weights and biases, generates the relubounds, distributes the shares
// REQUIRES model_shapes TO ALREADY BE SCALED
// model_path MUST INCLUDE "/csv/" AT END
void fullSetup(Lint **x, Lint ***weights, Lint ***biases, Lint **relu_bounds, string model_path, vector<string> weight_names, vector<string> bias_names, vector<vector<int>> model_shapes, vector<vector<int>> bias_dim, uint MAX_SCALE, uint ring_size, uint batch_size, NodeNetwork *nodeNet) {

    // uint i, j;
    // int pid = nodeNet->getID();
    uint num_layers = weight_names.size();

    vector<double> alphas;
    vector<double> betas;

    vector<vector<double>> in_weights;
    vector<vector<double>> in_biases;

    vector<vector<sLint>> v_weights;
    vector<vector<sLint>> v_biases;
    vector<sLint> v_relu_bounds;

    for (size_t i = 0; i < num_layers; i++) {
        // removed alpha parameter here becasue we already scaled and updated the model_shapes
        in_weights.push_back(reorderWeights(read_matrix(model_path + weight_names.at(i)), model_shapes.at(i).at(2), model_shapes.at(i).at(3), model_shapes.at(i).at(0)));
        in_biases.push_back(read_matrix(model_path + bias_names.at(i)));
    }

    alphas.push_back(MAX_SCALE);
    betas.push_back(MAX_SCALE);

    // starting from 1 since we have alpha_0 and beta_0
    for (size_t i = 1; i < num_layers; i++) {
        alphas.push_back(alphas.at(i - 1) * betas.at(i - 1));
        betas.push_back(getScale(MAX_SCALE, in_weights.at(i)));
    }

    for (size_t i = 0; i < num_layers; i++) {
        v_weights.push_back(q_applyScale(betas.at(i), in_weights.at(i)));
        v_biases.push_back(q_applyScale(betas.at(i) * alphas.at(i), in_biases.at(i)));
    }

    for (size_t i = 1; i < num_layers; i++) {
        v_relu_bounds.push_back(6 * alphas.at(i));
    }

    // at this point, input weights and biases have been scaled, and we have all the necessary alphas and betas
    // now we can invoke splitData and store everything in ***weights, ***biases, and so on

    for (size_t i = 0; i < num_layers; i++) {
        uint weight_size = model_shapes.at(i).at(0) * model_shapes.at(i).at(1) * model_shapes.at(i).at(2) * model_shapes.at(i).at(3);

        splitData(weights[i], v_weights.at(i), weight_size, ring_size, nodeNet);
        splitData(biases[i], v_biases.at(i), bias_dim.at(i).at(0), ring_size, nodeNet);
    }

    splitData(relu_bounds, v_relu_bounds, num_layers - 1, ring_size, nodeNet);
}

void q_fullSetup(
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
        splitData(weights[i], in_weights.at(i), weight_size, ring_size, nodeNet);
        splitData(biases[i], in_biases.at(i), bias_dim.at(i).at(0), ring_size, nodeNet);
    }

    splitData(input_zp, input_quant_params, num_layers, ring_size, nodeNet);
    splitData(weights_zp, w_quant_params, num_layers, ring_size, nodeNet);
    splitData(output_zp, output_quant_params, num_layers, ring_size, nodeNet);

    splitData(relu_bounds, in_relu_bounds, num_layers, ring_size, nodeNet);
    splitData(trunc_vals, pre_trunc_vals, num_layers, ring_size, nodeNet);
    splitData_X(x, in_x, input_dim * input_dim * 3, batch_size, ring_size, nodeNet);
}

