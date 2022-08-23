#include "../include/neural_ops.h"

// does the actual "convolution" step required prior to matrix multiplication
// ksize = kernelsize
// we assume the size of res is sufficient to handle
void im2col(Lint **res, Lint **a, uint channels, uint height, uint width, uint ksize, uint stride, uint batch_size,  NodeNetwork *nodeNet) {

    uint pad, c, h, w, height_col, width_col, channels_col, w_offset, h_offset, c_im, im_row, im_col, col_index, in_batch_size, out_batch_size;
    uint numShares = nodeNet->getNumShares();

    for (uint i = 0; i < batch_size; i++) {
        pad = 0;

        height_col = (height + 2 * pad - ksize) / stride + 1;
        width_col = (width + 2 * pad - ksize) / stride + 1;
        channels_col = channels * ksize * ksize;

        // used for batch optimization
        in_batch_size = channels * height * width;
        out_batch_size = channels_col * height_col * width_col;

        for (c = 0; c < channels_col; ++c) {
            w_offset = c % ksize;
            h_offset = (c / ksize) % ksize;
            c_im = c / ksize / ksize;
            for (h = 0; h < height_col; ++h) {
                for (w = 0; w < width_col; ++w) {

                    im_row = h_offset + h * stride;
                    im_col = w_offset + w * stride;
                    col_index = (c * height_col + h) * width_col + w;

                    if (im_row < 0 || im_col < 0 || (im_row >= height) || (im_col >= width)) {
                        for (size_t s = 0; s < numShares; s++)
                            res[s][col_index] = 0;
                        // res[1][col_index] = 0;

                        // check if correct
                        // res[s][col_index + i * out_batch_size] = 0;
                        // res[1][col_index + i * out_batch_size] = 0;

                    } else {
                        for (size_t s = 0; s < numShares; s++)
                            res[s][col_index + i * out_batch_size] = a[s][i * in_batch_size + im_col + width * (im_row + height * c_im)];
                        // res[1][col_index + i * out_batch_size] = a[1][i * in_batch_size + im_col + width * (im_row + height * c_im)];
                    }
                }
            }
        }
    }
}

void extract_patch(Lint **res, Lint **data_im, int channels, int height, int width, int ksize, int stride, uint batch_size, int pad_flag, NodeNetwork *nodeNet) {

    int c, h, w, pad, pad_along_height, pad_along_width, pad_top, pad_bottom, pad_left, pad_right, height_col, width_col, channels_col;
    uint numShares = nodeNet->getNumShares();

    if (pad_flag == 0) {
        // we have VALID padding (so none at all)
        pad_top = 0;
        pad_left = 0;
        pad = 0;

        height_col = floor((height + 2 * pad - ksize) / stride + 1);
        width_col = floor((width + 2 * pad - ksize) / stride + 1);

    } else {
        // SAME PADDING

        height_col = ceil(float(height) / stride);
        width_col = ceil(float(width) / stride);

        if ((height % stride) == 0) {
            pad_along_height = std::max(ksize - stride, 0);

        } else {
            pad_along_height = std::max(ksize - (height % stride), 0);
        }
        if ((width % stride) == 0) {
            pad_along_width = std::max(ksize - stride, 0);

        } else {
            pad_along_width = std::max(ksize - (width % stride), 0);
        }

        if (!(stride > 1)) {

            pad_top = floor(float(pad_along_height) / 2.0);
            pad_bottom = pad_along_height - pad_top;
            pad_left = floor((float(pad_along_width) / 2.0));
            pad_right = pad_along_width - pad_left;

        } else {
            pad_left = 0;
            pad_top = 0;
        }
    }

    channels_col = channels * width_col * height_col;

    uint in_batch_size = channels * height * width;

    uint out_batch_size = ksize * ksize * channels * width_col * height_col;

    for (uint i = 0; i < batch_size; i++) {

        for (c = 0; c < width_col * height_col; ++c) {
            int w_offset = c % width_col;
            int h_offset = (c / height_col) % height_col;
            int c_im = c / width_col / width_col;

            for (h = 0; h < ksize; ++h) {
                for (w = 0; w < ksize * channels; ++w) {
                    int im_row = h_offset * stride + h;
                    int im_col = w_offset * stride * channels + w;
                    int col_index = (c * ksize + h) * ksize * channels + w;

                    im_row -= pad_top;
                    im_col -= pad_left;

                    if (im_row < 0 || im_col < 0 || (im_row >= height) || (im_col >= width * channels)) {
                        for (size_t s = 0; s < numShares; s++)
                            res[s][col_index + i * out_batch_size] = 0;
                        // res[1][col_index + i * out_batch_size] = 0;

                    } else {
                        for (size_t s = 0; s < numShares; s++)
                            res[s][col_index + i * out_batch_size] = data_im[s][i * in_batch_size + im_col + width * channels * (im_row + height * c_im)];
                        // res[1][col_index + i * out_batch_size] = data_im[1][i * in_batch_size + im_col + width * channels * (im_row + height * c_im)];
                    }
                }
            }
        }
    }
}

void pad_original(Lint **res, Lint **data_im, int channels, int height, int width, int ksize, int stride, uint batch_size, NodeNetwork *nodeNet) {

    // int padw = (ksize / 2);
    uint numShares = nodeNet->getNumShares();

    int c, h, w;

    int height_col = height + 2;
    int width_col = width + 2;

    int pad_left = channels;
    int pad_top = 1;
    int channels_col = channels;

    // vector<double> data_col((height_col) * (width_col)*channels, 0);

    uint in_batch_size = channels * height * width;
    uint out_batch_size = channels_col * height_col * width_col;

    uint w_offset, h_offset, c_im, im_row, im_col, col_index;

    for (uint i = 0; i < batch_size; i++) {

        for (c = 0; c < 1; ++c) {
            w_offset = c % width_col;
            h_offset = (c / height_col) % height_col;
            c_im = c / width_col / width_col;

            for (h = 0; h < height_col; ++h) {
                for (w = 0; w < width_col * channels; ++w) {
                    im_row = h_offset * stride + h;
                    im_col = w_offset * stride * channels + w;
                    col_index = (c * height_col + h) * width_col * channels + w;

                    im_row -= pad_top;
                    im_col -= pad_left;

                    if (im_row < 0 || im_col < 0 || (im_row >= height) || (im_col >= width * channels)) {
                        for (size_t s = 0; s < numShares; s++)
                            res[s][col_index + i * out_batch_size] = 0;
                        // res[1][col_index + i * out_batch_size] = 0;

                    } else {
                        for (size_t s = 0; s < numShares; s++)
                            res[s][col_index + i * out_batch_size] = data_im[s][i * in_batch_size + im_col + width * channels * (im_row + height * c_im)];
                        // res[1][col_index + i * out_batch_size] = data_im[1][i * in_batch_size + im_col + width * channels * (im_row + height * c_im)];
                    }

                    // data_col[col_index] = pad_get_pixel(data_im, height, width, channels, im_row, im_col, c_im, pad_top, pad_left);
                }
            }
        }
    }
}

// computes the 2d depthwise convolution on an input tensor x w/ weight matrix W

void MaxPool(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size,  NodeNetwork *nodeNet, uint flag) {
    if (flag == 0) {
        old_MaxPool(res, a, c, m, n, batch_size, ring_size, nodeNet);
    } else {
        eda_MaxPool(res, a, c, m, n, batch_size, ring_size, nodeNet);
    }
}

void old_MaxPool(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size,  NodeNetwork *nodeNet) {
    // takes in matrix of dimension m x n and pools according to the window
    // output matrix dimensions are calculated based off of inputs
    // we exclude the z direction in function since it is one in all cases
    uint i, j, k, l; //index; // used for loops
    // uint bytes = ((ring_size) + 7) >> 3;
    uint numShares = nodeNet->getNumShares();

    // uint size = (c*m*n)/2;
    uint size = (c * m * n) / 2;
    // printf("size : %u\n", size);

    // Lint *res2 = new Lint [100000];
    // memset(res2,0,sizeof(Lint)*100000);

    Lint **x0 = new Lint *[numShares];
    Lint **x1 = new Lint *[numShares];
    Lint **temp = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        x0[i] = new Lint[size * batch_size];
        memset(x0[i], 0, sizeof(Lint) * (size * batch_size));
        x1[i] = new Lint[size * batch_size];
        memset(x1[i], 0, sizeof(Lint) * (size * batch_size));
        temp[i] = new Lint[size * batch_size];
        memset(temp[i], 0, sizeof(Lint) * (size * batch_size));
    }

    for (j = 0; j < batch_size; j++) {

        for (i = 0; i < size; i++) {
            for (size_t s = 0; s < numShares; s++) {

                x0[s][j * size + i] = a[s][j * size * 2 + 2 * i];
                // x0[1][j * size + i] = a[1][j * size * 2 + 2 * i];
                x1[s][j * size + i] = a[s][j * size * 2 + 2 * i + 1];
                // x1[1][j * size + i] = a[1][j * size * 2 + 2 * i + 1];
            }
        }
    }

    Rss_LT(temp, x0, x1, size * batch_size, ring_size, nodeNet); // c is the problem

    for (i = 0; i < numShares; i++) {
        memset(x0[i], 0, sizeof(Lint) * (size * batch_size));
        memset(x1[i], 0, sizeof(Lint) * (size * batch_size));
        // memset(res[i],0,sizeof(Lint)*(size));
    }
    size = size / 2;
    // printf("size : %u\n", size);

    // Rss_Open(res2, temp, 16*12*12, ring_size, nodeNet);
    // for(int i =0; i< 30; i++) {
    //     printf("res[%i]  : %llu\n", i, res2[i]);
    // }
    for (l = 0; l < batch_size; l++) {
        k = 0;
        for (i = 0; i < c * m / 2; i++) {
            for (j = 0; j < n / 2; j++) {
                for (size_t s = 0; s < numShares; s++) {

                    // printf("j + i*n : %u\n", j + i*n);
                    // printf("j + m/2 + i*n : %u\n", j + m/2 +i*n);
                    x0[s][l * size + k] = temp[s][2 * l * size + j + i * n];
                    // x0[1][l * size + k] = temp[1][2 * l * size + j + i * n];
                    x1[s][l * size + k] = temp[s][2 * l * size + j + m / 2 + i * n];
                    // x1[1][l * size + k] = temp[1][2 * l * size + j + m / 2 + i * n];
                }

                k++;
            }
        }
    }

    Rss_LT(res, x0, x1, size * batch_size, ring_size, nodeNet);

    // delete [] res2;
    // cleanup
    for (i = 0; i < numShares; i++) {
        delete[] x0[i];
        delete[] temp[i];
        delete[] x1[i];
    }
    delete[] x0;
    delete[] x1;
    delete[] temp;
}

void eda_MaxPool(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size,  NodeNetwork *nodeNet) {
    // takes in matrix of dimension m x n and pools according to the window
    // output matrix dimensions are calculated based off of inputs
    // we exclude the z direction in function since it is one in all cases
    uint i, j, k, l; //index; // used for loops
    // uint bytes = ((ring_size) + 7) >> 3;
    uint numShares = nodeNet->getNumShares();

    // uint size = (c*m*n)/2;
    uint size = (c * m * n) / 2;
    // printf("size : %u\n", size);

    // Lint *res2 = new Lint [100000];
    // memset(res2,0,sizeof(Lint)*100000);

    Lint **x0 = new Lint *[numShares];
    Lint **x1 = new Lint *[numShares];
    Lint **temp = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        x0[i] = new Lint[size * batch_size];
        memset(x0[i], 0, sizeof(Lint) * (size * batch_size));
        x1[i] = new Lint[size * batch_size];
        memset(x1[i], 0, sizeof(Lint) * (size * batch_size));
        temp[i] = new Lint[size * batch_size];
        memset(temp[i], 0, sizeof(Lint) * (size * batch_size));
    }

    for (j = 0; j < batch_size; j++) {

        for (i = 0; i < size; i++) {
            for (size_t s = 0; s < numShares; s++) {
                x0[s][j * size + i] = a[s][j * size * 2 + 2 * i];
                x1[s][j * size + i] = a[s][j * size * 2 + 2 * i + 1];
            }
            // x0[0][j * size + i] = a[0][j * size * 2 + 2 * i];
            // x0[1][j * size + i] = a[1][j * size * 2 + 2 * i];
            // x1[0][j * size + i] = a[0][j * size * 2 + 2 * i + 1];
            // x1[1][j * size + i] = a[1][j * size * 2 + 2 * i + 1];
        }
    }

    new_Rss_LT(temp, x0, x1, size * batch_size, ring_size, nodeNet); // c is the problem

    for (i = 0; i < numShares; i++) {
        memset(x0[i], 0, sizeof(Lint) * (size * batch_size));
        memset(x1[i], 0, sizeof(Lint) * (size * batch_size));
        // memset(res[i],0,sizeof(Lint)*(size));
    }
    size = size / 2;
    // printf("size : %u\n", size);

    // Rss_Open(res2, temp, 16*12*12, ring_size, nodeNet);
    // for(int i =0; i< 30; i++) {
    //     printf("res[%i]  : %llu\n", i, res2[i]);
    // }
    for (l = 0; l < batch_size; l++) {
        k = 0;
        for (i = 0; i < c * m / 2; i++) {
            for (j = 0; j < n / 2; j++) {

                for (size_t s = 0; s < numShares; s++) {
                    // printf("j + i*n : %u\n", j + i*n);
                    // printf("j + m/2 + i*n : %u\n", j + m/2 +i*n);
                    x0[s][l * size + k] = temp[s][2 * l * size + j + i * n];
                    // x0[1][l * size + k] = temp[1][2 * l * size + j + i * n];
                    x1[s][l * size + k] = temp[s][2 * l * size + j + m / 2 + i * n];
                    // x1[1][l * size + k] = temp[1][2 * l * size + j + m / 2 + i * n];
                }
                k++;
            }
        }
    }

    new_Rss_LT(res, x0, x1, size * batch_size, ring_size, nodeNet);

    // delete [] res2;
    // cleanup
    for (i = 0; i < numShares; i++) {
        delete[] x0[i];
        delete[] temp[i];
        delete[] x1[i];
    }
    delete[] x0;
    delete[] x1;
    delete[] temp;
}

void ReLU(Lint **res, Lint **a, uint size, uint ring_size,  NodeNetwork *nodeNet, uint flag) {
    if (flag == 0) {
        old_ReLU(res, a, size, ring_size, nodeNet);
    } else {
        eda_ReLU(res, a, size, ring_size, nodeNet);
    }
}

// does not need to be changed for batch optimization
void old_ReLU(Lint **res, Lint **a, uint size, uint ring_size,  NodeNetwork *nodeNet) {
    // takes in matrix of dimension m x n and calculates the ReLU for each element
    // output dimensions same as input
    uint i; // j, k, index; // used for loops
    // uint bytes = ((ring_size) + 7) >> 3;
    uint numShares = nodeNet->getNumShares();

    Lint **zero = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        zero[i] = new Lint[size];
        memset(zero[i], 0, sizeof(Lint) * (size));
    }
    Rss_LT(res, a, zero, size, ring_size, nodeNet);
    // cleanup
    for (i = 0; i < numShares; i++) {
        delete[] zero[i];
    }
    delete[] zero;
}

// does not need to be changed for batch optimization
void eda_ReLU(Lint **res, Lint **a, uint size, uint ring_size,  NodeNetwork *nodeNet) {
    // takes in matrix of dimension m x n and calculates the ReLU for each element
    // output dimensions same as input
    uint i; // used for loops
    uint numShares = nodeNet->getNumShares();

    Lint **zero = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        zero[i] = new Lint[size];
        memset(zero[i], 0, sizeof(Lint) * (size));
    }
    new_Rss_LT(res, a, zero, size, ring_size, nodeNet);
    // cleanup
    for (i = 0; i < numShares; i++) {
        delete[] zero[i];
    }
    delete[] zero;
}

// upper will be ALL of the alpha*6 values for the network
// we compare all the x's to upper[layer]
void ReLU_6_alpha_timer(Lint **res, Lint **x, Lint **upper, uint size, uint ring_size, uint layer,  NodeNetwork *nodeNet, unsigned long &timer) {
    
    fixed_Rss_GT_LT_time(res, res, upper, size, ring_size, layer, nodeNet, timer);

}


void ReLU_6_alpha(Lint **res, Lint **x, Lint **upper, uint size, uint ring_size, uint layer,  NodeNetwork *nodeNet) {
    uint i;
    uint numShares = nodeNet->getNumShares();

    Lint **zero = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        zero[i] = new Lint[1];
        memset(zero[i], 0, sizeof(Lint) * (1));
    }
    // must be done sequentially
    // first compare with zero
    fixed_Rss_LT(res, x, zero, size, ring_size, 0, nodeNet);
    // then compare with 6*alpha
    fixed_Rss_GT(res, res, upper, size, ring_size, layer, nodeNet);

    // cleanup
    for (i = 0; i < numShares; i++) {
        delete[] zero[i];
    }
    delete[] zero;
}

void add_biases(Lint **res, Lint **a, Lint **b, uint m, uint n, uint batch_size, NodeNetwork *nodeNet) {

    uint i, j, k;
    uint numShares = nodeNet->getNumShares();

    for (k = 0; k < batch_size; k++) {
        for (i = 0; i < m; i++) {
            for (j = 0; j < n; j++) {
                for (size_t s = 0; s < numShares; s++)
                    res[s][k * m * n + i * n + j] = a[s][k * m * n + i * n + j] + b[s][i];
                // res[1][k * m * n + i * n + j] = a[1][k * m * n + i * n + j] + b[1][i];
            }
        }
    }
}

void pw_add_biases(Lint **res, Lint **x, Lint **b, uint height, uint width, uint channels, uint batch_size, NodeNetwork *nodeNet) {

    uint i, k;
    uint numShares = nodeNet->getNumShares();

    uint total_size = height * width;

    for (k = 0; k < total_size * batch_size; k++) {
        for (i = 0; i < channels; i++) {
            for (size_t s = 0; s < numShares; s++)
                res[s][k * channels + i] = x[s][k * channels + i] + b[s][i];
            // res[1][k * channels + i] = x[1][k * channels + i] + b[1][i];
        }
    }
}

void sub_zp(Lint **res, Lint **x, Lint **z_p, uint size, uint batch_size, uint layer_id, NodeNetwork *nodeNet) {
    uint numShares = nodeNet->getNumShares();
    uint i;
    for (i = 0; i < size * batch_size; i++) {
        for (size_t s = 0; s < numShares; s++)
            res[s][i] = x[s][i] - z_p[s][layer_id];
        // res[1][i] = x[1][i] - z_p[1][layer_id];
    }
}

void add_zp(Lint **res, Lint **x, Lint **z_p, uint size, uint batch_size, uint layer_id, NodeNetwork *nodeNet) {
    uint numShares = nodeNet->getNumShares();
    uint i;
    for (i = 0; i < size * batch_size; i++) {
        for (size_t s = 0; s < numShares; s++)
            res[s][i] = x[s][i] + z_p[s][layer_id];
        // res[1][i] = x[1][i] + z_p[1][layer_id];
    }
}

// does NOT compute the average, just sums the results (no division)
void Rss_avg_pool(Lint **res, Lint **x, uint in_channels, uint height, uint width, uint kernel, uint stride, uint batch_size, NodeNetwork *nodeNet) {
    uint input_dim = in_channels * width * height;
    uint numShares = nodeNet->getNumShares();

    uint i, j, k, l;

    // destination must be sanitized
    for (i = 0; i < numShares; i++) {
        memset(res[i], 0, sizeof(Lint) * input_dim * batch_size);
    }

    uint out_index = 0;
    for (l = 0; l < batch_size; l++) {
        for (i = 0; i < in_channels; i++) {
            for (j = 0; j < width; j++) {
                for (k = 0; k < height; k++) {
                    res[0][out_index] += x[0][k * in_channels + j * height * in_channels + i + l * input_dim];
                    res[1][out_index] += x[1][k * in_channels + j * height * in_channels + i + l * input_dim];
                }
            }
            out_index++;
        }
    }
}

// does NOT compute the average, just sums the results (no division)
void Rss_avg_pool_trunc(Lint **res, Lint **x, uint in_channels, uint *n, uint kernel, uint stride, uint batch_size, uint ring_size,  NodeNetwork *nodeNet, unsigned long &timer) {
    uint input_dim = in_channels * (*n) * (*n);
    uint out_dim = in_channels * batch_size;

    Lint t_prime = log2((*n) * (*n));
    uint numShares = nodeNet->getNumShares();

    uint i, j, k, l;

    // destination must be sanitized
    for (i = 0; i < numShares; i++) {
        memset(res[i], 0, sizeof(Lint) * out_dim);
    }

    uint out_index = 0;
    for (l = 0; l < batch_size; l++) {
        for (i = 0; i < in_channels; i++) {
            for (j = 0; j < (*n); j++) {
                for (k = 0; k < (*n); k++) {
                    for (size_t s = 0; s < numShares; s++)
                        res[s][out_index] += x[s][k * in_channels + j * (*n) * in_channels + i + l * input_dim];
                    // res[1][out_index] += x[1][k * in_channels + j * (*n) * in_channels + i + l * input_dim];
                }
            }
            out_index++;
        }
    }

    Rss_truncPr_time(res, res, t_prime, out_dim, ring_size, nodeNet, timer);

    // updating size of n
    *n = (*n / *n);
}