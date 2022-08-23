#include "Open.h"

void Rss_Open(Lint *res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) {
    // a[0] will be sent to next neighbor map[0], res will be filled by the received
    // value from map[1] the r_size in the functions that including open
    // functionality (open_s, multpub) refers to the ring size that we are doing
    // computation over, this might be different from the basic ring_size. E.g., in
    // randbits, multpub is working over ring_size+2.
    // communication
    int i;
    // uint bytes = (RING[ring_size] + 7)>>3;
    // for(i = 0; i<size; i++){
    // a[1] = a[1] & nodeNet->SHIFT[maskring_size];
    //}
    // }

    nodeNet->SendAndGetDataFromPeer(a[1], res, size, ring_size);
    for (i = 0; i < size; i++) {
        res[i] = a[0][i] + a[1][i] + res[i];
        res[i] = res[i] & nodeNet->SHIFT[ring_size];
        // res[i] = bitExtracted(res[i], nodeNet->RING[ring_size]);
    }
}

void Rss_Open_Byte(uint8_t *res, uint8_t **a, uint size, uint ring_size, NodeNetwork *nodeNet) {
    // a[0] will be sent to next neighbor map[0], res will be filled by the received
    // value from map[1] the r_size in the functions that including open
    // functionality (open_s, multpub) refers to the ring size that we are doing
    // computation over, this might be different from the basic ring_size. E.g., in
    // randbits, multpub is working over ring_size+2.
    // communication
    int i;
    // uint bytes = (size+8-1)>>3;  //number of bytes need to be send/recv

    // uint bytes = (RING[ring_size] + 7)>>3;
    // for(i = 0; i<size; i++){
    // a[1] = a[1] & nodeNet->SHIFT[maskring_size];
    //}
    nodeNet->SendAndGetDataFromPeer_bit(a[1], res, size);
    for (i = 0; i < size; i++) {
        res[i] = a[0][i] ^ a[1][i] ^ res[i];
        // res[i] = res[i] & nodeNet->SHIFT[ring_size];
        // res[i] = bitExtracted(res[i], nodeNet->RING[ring_size]);
    }
}

void Rss_Open_Bitwise(Lint *res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) {
    // a[0] will be sent to next neighbor map[0], res will be filled by the received
    // value from map[1] the r_size in the functions that including open
    // functionality (open_s, multpub) refers to the ring size that we are doing
    // computation over, this might be different from the basic ring_size. E.g., in
    // randbits, multpub is working over ring_size+2.
    // communication
    int i;
    // uint bytes = (RING[ring_size] + 7)>>3;
    // for(i = 0; i<size; i++){
    // a[1] = a[1] & nodeNet->SHIFT[maskring_size];
    //}
    nodeNet->SendAndGetDataFromPeer(a[1], res, size, ring_size);
    for (i = 0; i < size; i++) {
        res[i] = a[0][i] ^ a[1][i] ^ res[i];
        res[i] = res[i] & nodeNet->SHIFT[ring_size];
        // res[i] = bitExtracted(res[i], nodeNet->RING[ring_size]);
    }
}