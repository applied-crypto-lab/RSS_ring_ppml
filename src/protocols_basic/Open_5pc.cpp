#include "Open_5pc.h"

void Rss_Open_mp(Lint *res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) {
    // a[0] will be sent to next neighbor map[0], res will be filled by the received
    // value from map[1] the r_size in the functions that including open
    // functionality (open_s, multpub) refers to the ring size that we are doing
    // computation over, this might be different from the basic ring_size. E.g., in
    // randbits, multpub is working over ring_size+2.
    // sanitizing
    // communication
    uint threshold = nodeNet->getThreshold();
    int i;
    Lint **recvbuf = new Lint *[threshold];
    Lint **sendbuf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recvbuf[i] = new Lint[size];
        memset(recvbuf[i], 0, sizeof(Lint) * size);
        sendbuf[i] = new Lint[size];
        memset(sendbuf[i], 0, sizeof(Lint) * size);
    }
    for (i = 0; i < size; i++) {
        sendbuf[0][i] = a[0][i] + a[1][i];
        sendbuf[1][i] = a[4][i] + a[5][i];
    }

    nodeNet->SendAndGetDataFromPeer_Open(sendbuf, recvbuf, size, ring_size);
    for (i = 0; i < size; i++) {
        res[i] = a[0][i] + a[1][i] + a[2][i] + a[3][i] + a[4][i] + a[5][i] + recvbuf[0][i] + recvbuf[1][i];
        res[i] = res[i] & nodeNet->SHIFT[ring_size];
    }
    for (i = 0; i < threshold; i++) {
        delete[] recvbuf[i];
        delete[] sendbuf[i];
    }
    delete[] recvbuf;
    delete[] sendbuf;
}

void Rss_Open_Byte_mp(uint8_t *res, uint8_t **a, uint size, uint ring_size, NodeNetwork *nodeNet) {
    
    uint threshold = nodeNet->getThreshold();
    int i;
    uint8_t **recvbuf = new uint8_t *[threshold];
    uint8_t **sendbuf = new uint8_t *[threshold];
    for (i = 0; i < threshold; i++) {
        recvbuf[i] = new uint8_t[size];
        sendbuf[i] = new uint8_t[size];
        memset(recvbuf[i], 0, sizeof(uint8_t) * size);
        memset(sendbuf[i], 0, sizeof(uint8_t) * size);
    }

    for (i = 0; i < size; i++) {
        sendbuf[0][i] = a[0][i] ^ a[1][i];
        sendbuf[1][i] = a[4][i] ^ a[5][i];
    }

    nodeNet->SendAndGetDataFromPeer_bit_Open(sendbuf, recvbuf, size);
    for (i = 0; i < size; i++) {
        res[i] = a[0][i] ^ a[1][i] ^ a[2][i] ^ a[3][i] ^ a[4][i] ^ a[5][i] ^ recvbuf[0][i] ^ recvbuf[1][i];
    }
    for (i = 0; i < threshold; i++) {
        delete[] recvbuf[i];
        delete[] sendbuf[i];
    }
    delete[] recvbuf;
    delete[] sendbuf;
}

void Rss_Open_Bitwise_mp(Lint *res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) {
    // a[0] will be sent to next neighbor map[0], res will be filled by the received
    // value from map[1] the r_size in the functions that including open
    // functionality (open_s, multpub) refers to the ring size that we are doing
    // computation over, this might be different from the basic ring_size. E.g., in
    // randbits, multpub is working over ring_size+2.
    // communication
    uint threshold = nodeNet->getThreshold();
    int i;
    Lint **recvbuf = new Lint *[threshold];
    Lint **sendbuf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recvbuf[i] = new Lint[size];
        memset(recvbuf[i], 0, sizeof(Lint) * size);
        sendbuf[i] = new Lint[size];
        memset(sendbuf[i], 0, sizeof(Lint) * size);
    }
    for (i = 0; i < size; i++) {
        sendbuf[0][i] = a[0][i] ^ a[1][i];
        sendbuf[1][i] = a[4][i] ^ a[5][i];
    }

    nodeNet->SendAndGetDataFromPeer_Open(sendbuf, recvbuf, size, ring_size);
    for (i = 0; i < size; i++) {
        res[i] = a[0][i] ^ a[1][i] ^ a[2][i] ^ a[3][i] ^ a[4][i] ^ a[5][i] ^ recvbuf[0][i] ^ recvbuf[1][i];
        res[i] = res[i] & nodeNet->SHIFT[ring_size];
    }
    for (i = 0; i < threshold; i++) {
        delete[] recvbuf[i];
        delete[] sendbuf[i];
    }
    delete[] recvbuf;
    delete[] sendbuf;
}
