#include "aes_ni.h"

#define KE2(NK,OK,RND) NK = OK; \
    NK = _mm_xor_si128(NK, _mm_slli_si128(NK, 4));  \
    NK = _mm_xor_si128(NK, _mm_slli_si128(NK, 4));  \
    NK = _mm_xor_si128(NK, _mm_slli_si128(NK, 4));  \
    NK = _mm_xor_si128(NK, _mm_shuffle_epi32(_mm_aeskeygenassist_si128(OK, RND), 0xff)); // BAD INSTRUCTION HERE ////////////////////

void offline_prg(uint8_t * dest, uint8_t * src, void * ri) {
	__m128i or, mr;
	__m128i * r = ri;

    or = _mm_load_si128((__m128i *) src);
    mr = or;

    mr = _mm_xor_si128(mr, r[0]);

    mr = _mm_aesenc_si128(mr, r[1]);
    mr = _mm_aesenc_si128(mr, r[2]);
    mr = _mm_aesenc_si128(mr, r[3]);
    mr = _mm_aesenc_si128(mr, r[4]);
    mr = _mm_aesenc_si128(mr, r[5]);
    mr = _mm_aesenc_si128(mr, r[6]);
    mr = _mm_aesenc_si128(mr, r[7]);
    mr = _mm_aesenc_si128(mr, r[8]);
    mr = _mm_aesenc_si128(mr, r[9]);
    mr = _mm_aesenclast_si128(mr, r[10]);
    mr = _mm_xor_si128(mr, or);
    _mm_storeu_si128((__m128i*) dest, mr);

}

__m128i * offline_prg_keyschedule(uint8_t * src) {
	__m128i * r = malloc(11*sizeof(__m128i));

    r[0] = _mm_load_si128((__m128i *) src);

	KE2(r[1], r[0], 0x01)
	KE2(r[2], r[1], 0x02)
	KE2(r[3], r[2], 0x04)
	KE2(r[4], r[3], 0x08)
	KE2(r[5], r[4], 0x10)
	KE2(r[6], r[5], 0x20)
	KE2(r[7], r[6], 0x40)
	KE2(r[8], r[7], 0x80)
	KE2(r[9], r[8], 0x1b)
	KE2(r[10], r[9], 0x36)

	return r;
}

void prg_aes_ni(Lint* destination, uint8_t * seed, __m128i * key){
	uint8_t res[16] = {};

	offline_prg(res, seed, key);
	memset(seed, 0, 16);
	memset(destination, 0, sizeof(Lint));
	memcpy(seed, res, sizeof(Lint)); //cipher becomes new seed or key
	memcpy(destination, res, sizeof(Lint)); //cipher becomes new seed or key
}


void prg_aes_ni_byte(uint8_t* destination, uint8_t * seed, __m128i * key){
	uint8_t res[16] = {};

	offline_prg(res, seed, key);
	memset(seed, 0, 16);
	memset(destination, 0, sizeof(uint8_t));
	memcpy(seed, res, sizeof(uint8_t)); //cipher becomes new seed or key
	memcpy(destination, res, sizeof(uint8_t)); //cipher becomes new seed or key
}
void test_aes(){
	printf("AES test");
}


/*
** Using documented GCC type unsigned __int128 instead of undocumented
** obsolescent typedef name __uint128_t.  Works with GCC 4.7.1 but not
** GCC 4.1.2 (but __uint128_t works with GCC 4.1.2) on Mac OS X 10.7.4.
*/


/*      UINT64_MAX 18446744073709551615ULL */
#define P10_UINT64 10000000000000000000ULL   /* 19 zeroes */
#define E10_UINT64 19

#define STRINGIZER(x)   # x
#define TO_STRING(x)    STRINGIZER(x)

int print_u128_u(__uint128_t u128)
{
    int rc;
    if (u128 > UINT64_MAX)
    {
        __uint128_t leading  = u128 / P10_UINT64;
        __uint64_t  trailing = u128 % P10_UINT64;
        rc = print_u128_u(leading);
        rc += printf("%." TO_STRING(E10_UINT64) PRIu64, trailing);
    }
    else
    {
        uint64_t u64 = u128;
        rc = printf("%" PRIu64, u64);
    }
    return rc;
}

void print_128(__uint128_t *A, int size){
        int i;
	for(i = 0;i<size;i++){
	print_u128_u(A[i]);
	printf("\n");
	}
}

int print_u128_u2(__uint128_t x){
printf("__int128 max  %016"PRIx64"%016"PRIx64"\n",(uint64_t)(x>>64),(uint64_t)x);
return 0;
}

void print_1283(__uint128_t A){
	uint8_t plain[]      = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
	memcpy(plain, &A, 16);
		printf("128 is:");
        int i;
	for(i = 0; i<16; i++){

		printf("%d",plain[i]);
	}
	printf("\n");
}


