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

// #ifndef AES_NI_H
// #define AES_NI_H
// #include <stdio.h>
// #include <unistd.h>
// #include <stdint.h>     //for int8_t
// #include <string.h>     //for memcmp
// #include <tmmintrin.h>
// #include <wmmintrin.h>  //for intrinsics for AES-NI
// #include <inttypes.h>
// #include <unistd.h>
// #include <stdint.h>     //for int8_t
// #include <wmmintrin.h>  //for intrinsics for AES-NI
// #include <inttypes.h>
// #include "setringsize.h"

// void offline_prg(uint8_t * dest, uint8_t * src, void * ri);
// __m128i * offline_prg_keyschedule(uint8_t * src);
// void prg_aes_ni(Lint* destination, uint8_t * seed, __m128i * key);
// void prg_aes_ni_byte(uint8_t* destination, uint8_t * seed, __m128i * key);

// void test_aes();
// int print_u128_u(__uint128_t);
// int print_u128_u2(__uint128_t);
// void print_128(__uint128_t *A, int size);
// void print_1283(__uint128_t A);
// #endif


#ifndef AES_NI_H
#define AES_NI_H


#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>     //for int8_t
#include <string.h>     //for memcmp
#include <tmmintrin.h>
#include <wmmintrin.h>  //for intrinsics for AES-NI
#include <inttypes.h>
#include <unistd.h>
#include <stdint.h>     //for int8_t
#include <wmmintrin.h>  //for intrinsics for AES-NI
#include <inttypes.h>
#include "setringsize.h"

void offline_prg(uint8_t * dest, uint8_t * src, void * ri);
__m128i * offline_prg_keyschedule(uint8_t * src);
void prg_aes_ni(Lint* destination, uint8_t * seed, __m128i * key);
void prg_aes_ni_byte(uint8_t* destination, uint8_t * seed, __m128i * key);

void test_aes();
int print_u128_u(__uint128_t);
int print_u128_u2(__uint128_t);
void print_128(__uint128_t *A, int size);
void print_1283(__uint128_t A);

#ifdef __cplusplus
}
#endif

#endif
