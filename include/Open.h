#ifndef OPEN_H_
#define OPEN_H_

#include "NodeNetwork.h"
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

void Rss_Open(Lint *res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet);
void Rss_Open_Byte(uint8_t *res, uint8_t **a, uint size);
void Rss_Open_Bitwise(Lint *res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet);

#endif
