#ifndef OPEN_5PC_H_
#define OPEN_5PC_H_

#include "NodeNetwork.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_Open_mp(Lint *res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Open_Byte_mp(uint8_t *res, uint8_t **a, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Open_Bitwise_mp(Lint *res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet);

#endif
