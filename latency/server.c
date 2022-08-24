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

/*                                                                             
 *  A server receiving and sending back a message multiple times.              
 *  Usage: ./server.out -p <port> -n <message_size (bytes)>                    
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#include "connection.h"

void error(char *msg)
{
    perror(msg);
    exit(1);
}
int main(int argc, char *argv[])
{
    int sockfd, newsockfd;
    struct Config config = get_config(argc, argv);
    uint8_t *buffer = malloc(config.n_bytes);
    struct sockaddr_in serv_addr, cli_addr;

    // Create listening socket                                                 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(config.port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }
    printf("Server ready, listening on port %d\n", config.port);
    fflush(stdout);
    listen(sockfd, 5);
    socklen_t clilen = sizeof(cli_addr);

    // Accept connection and set nonblocking and nodelay                       
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        error("ERROR on accept");
    }
    fcntl(newsockfd, F_SETFL, O_NONBLOCK);
    int flag = 1;
    setsockopt(newsockfd, IPPROTO_TCP, TCP_NODELAY, (void *)&flag, sizeof(int));
   // Receive-send loop                                                       
    printf("Connection accepted, ready to receive!\n");
    int i;
    for (i = 0; i < N_ROUNDS; i++) {
        receive_message(config.n_bytes, newsockfd, buffer);
        send_message(config.n_bytes, newsockfd, buffer);
    }
    printf("Done!\n");

    // Clean state                                                             
    close(sockfd);
    close(newsockfd);

    return 0;
}
