# Replicated Secret Sharing over a Ring
This is our implementation of the protocols and from our PETS 2023.1 paper "Multi-Party Replicated Secret Sharing over a Ring with Applications to Privacy-Preserving Machine Learning." In its current state, the implementation supports 3-, 5-, and 7-party computation in the semi-honest, honest majority setting.

## Requirements

- clang 11 or newer (tested with 11), or gcc 9 or newer (tested with 9). clang is recommended since it performs better.
- OpenSSL v1.1.1
- CryptoPP (tested with 5.6.4)

If you are interested in running any neural network experiments, you can download the ones used in the paper from [this link](https://drive.google.com/file/d/1loj9UjmFnKABVB8tLpRoIJUs2YdkFA_2/view?usp=sharing).

## Compilation

The implementation supports rings over $\mathbb{Z}_{2^k}$ for $k \leq 62$. To compile the code, run the commands below, which assume you're using clang 11. You can replace the values `CMAKE_C_COMPILER` and `CMAKE_CXX_COMPILER` with another compiler of your choosing.
```
mkdir build
cd build
```
The code uses different definitions of the type `Lint` depending on the chosen ring size `k` to aid in overall performance. If `k <= 30`, run

```
cmake -DCMAKE_BUILD_TYPE=Release -D CMAKE_C_COMPILER=clang-11 -D CMAKE_CXX_COMPILER=clang++-11 -DUSE_30=ON ..
```
Otherwise (`31 <= k <= 62`), run
```
cmake -DCMAKE_BUILD_TYPE=Release -D CMAKE_C_COMPILER=clang-11 -D CMAKE_CXX_COMPILER=clang++-11 -DUSE_30=ON ..
```

## Key generation

In order to run the code, you first need to generate ssh key pairs for each party; the public keys **must** be accessible by each computing party. To generate the key pairs, run the script:
```
./ssh_keygen.sh <n>
```
where `n` is the number of parties.

## Running the code

To run the code, edit the IP addresses in `runtime-config` file to reflect the machine(s) that will run the computation. Parties must start running the code in **descending order** by their ID, i.e. party `N` first, then party `N-1`, and so on. Party `i` would execute the command
```
./rss_nn <id> <config> <experiment>
```
where `id` is the unique identifier of party `i`, `config` is the name of the configuration file (such as `runtime-config`), and `experiment` is the experiment you wish to run: [`micro`](#microbenchmarks-micro),  [`minionn`](#minionn-network-evaluation-minionn),  and [`q_mobilenet`](#quantized-mobilenets-evaluation-q_mobilenet).

### Microbenchmarks (`micro`)

The `micro` argument runs a single microbenchmark for a given protocol, and is executed as follows:
```
./rss_nn <id> <config> micro <protocol> <ring_size> <size_of_data> <batch_size> <num_iterations>
```
The options are:
- `protocol` is the protocol you wish to run. The available options are 
  - `mult`
  - `mulpub`
  - `randbit`
  - `edabit`
  - `msb_rb`
  - `msb_eda`
  - `mat_mult`
- `ring_size` is the ring size `k` in bits (e.g. 30, 60)
- `size_of_data` is the size of the data to test. For `mat_mult`, the size is the total number of elements, which must have an integer square root (e.g. 100, 10000, 250000)
- `batch_size` is unused here, set to 1
- `num_iterations` is the number of times the computation is repeated, and therefore averaged over to eliminate any deviation

### MiniONN network evaluation (`minionn`)

The `minionn` argument runs the four-layer CNN from Liu~et~al.'s landmark 2017 paper "Oblivious Neural Network Predictions via MiniONN Transformations" (Figure 12). The program must be compiled using the `31 <= k <= 62` from above. To run the benchmark, execute the following command
```
./rss_nn <id> <config> minionn <num_images> <batch_size> <model_path>
```
The options are:
- `num_images` is the number of images you want to evaluate (in total)
- `batch_size` is the batch size of images you want to evaluate concurrently (e.g. 1, 5, 10)
- `model_path` is the path to the directory containing the models

### Quantized Mobilenets evaluation (`q_mobilenet`)

The `q_mobilenet` argument runs the inference using the quantized version of the Mobilenet neural network. The program must be compiled using the `k <= 30` command from above. To run the benchmark, execute the following command
```
./rss_nn <id> <config> q_mobilenet <input_dim> <alpha_index> <batch_size> <num_iterations> <num_discarded> <model_path>
```
The options are:
- `input_dim` is the dimension of the input image (128, 160, 192, 224)
- `alpha_index` is the index corresponding to the width multiplier $\alpha$ of the network (0.25, 0.5, 0.75, 1.0)
- `batch_size` is the batch size of images you want to evaluate concurrently (e.g. 1, 5, 10)
- `num_iterations` is the number of times the computation is repeated, and therefore averaged over to eliminate any deviation
- `num_discarded` is the number of runs which are discarded before recording the actual run times (i.e. the warmup period)
- `model_path` is the path to the directory containing the models