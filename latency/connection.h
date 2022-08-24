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

#include <errno.h>
#include <stdint.h>
#include <getopt.h> 
#include <stdio.h> 
#include <stdlib.h>

#define DEFAULT_N_BYTES 1
#define DEFAULT_PORT 8787
#define DEFAULT_ADDRESS "127.0.0.1"
#define N_ROUNDS 100

struct Config {
    char *address;
    int port;
    int n_bytes;
};

void print_config(struct Config config) {
    printf("Address: %s, Port: %d, N_bytes: %d\n", config.address, config.port\
, config.n_bytes);
}

// Parse command line args to extract config. Default values used when arg mis\
sing                                                                           
struct Config get_config(int argc, char *argv[]) {
    struct Config config;
    int c;
    config.n_bytes = DEFAULT_N_BYTES;
    config.port = DEFAULT_PORT;
    config.address = DEFAULT_ADDRESS;

    while ((c = getopt(argc, argv, "a:p:b:")) != -1) {
        switch (c) {
            case 'a':
                    config.address = optarg;
                    break;
            case 'p':
                    config.port = atoi(optarg);
                    break;
            case 'b':
                    config.n_bytes = atoi(optarg);
                    break;
            default: /* '?' */
                   fprintf(stderr, "Usage: %s [-b bytes] [-a address] [-p port\
]\n",
                           argv[0]);
                   exit(EXIT_FAILURE);
        }
    }
    print_config(config);
    return config;
}
void panic(char *msg)
{
    perror(msg);
    exit(0);
}

// Starts a measure                                                            
uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

// Obtains ticks successive to a rdtsc() call                                  
uint64_t rdtscp(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtscp" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}
// Reads from the given socket into the given buffer n_bytes bytes             
int receive_message(size_t n_bytes, int sockfd, uint8_t *buffer) {
    int bytes_read = 0;
    int r;
    while (bytes_read < n_bytes) {
        // Make sure we read exactly n_bytes                                   
        r = read(sockfd, buffer, n_bytes - bytes_read);
        if (r < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK)) {
            panic("ERROR reading from socket");
        }
        if (r > 0) {
            bytes_read += r;
        }
    }
    return bytes_read;
}

// Writes n_bytes from the given buffer to the given socekt                    
int send_message(size_t n_bytes, int sockfd, uint8_t *buffer) {
    int bytes_sent = 0;
    int r;
    while (bytes_sent < n_bytes) {
        // Make sure we write exactly n_bytes                                  
        r = write(sockfd, buffer, n_bytes - bytes_sent);
        if (r < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK)) {
            panic("ERROR writing to socket");
        }
        if (r > 0) {
            bytes_sent += r;
        }
    }
    return bytes_sent;
}
