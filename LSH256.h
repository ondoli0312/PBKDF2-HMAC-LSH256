#pragma once
#include "Type.h"

typedef struct
{
	word lsh[16];
	word byte_msglen;
	word buf[32];
}LSH256_INFO;

void MsgExp(IN word Msgblock[32], OUT word output[27][16]);
void Mix_even_with_permu(word T[16], int Ns, word output[16]);
void Mix_odd_with_permu(word T[16], int Ns, word output[16]);
void Compression(LSH256_INFO* info);
void LSH256_init(OUT LSH256_INFO* info);
void LSH256_Process(IN byte* msg, IN word byte_msglen, OUT LSH256_INFO* info);
void LSH256_Final(IN LSH256_INFO* info, OUT byte* output);
void LSH256(IN const byte* msg, IN word msg_len, OUT byte* output);

//SIMD
#include <emmintrin.h>

#define LSH_ALIGNED_(x) __declspec(align(x))
#define LOAD(x) _mm_loadu_si128((__m128i*)x)
#define STORE(x,y) _mm_storeu_si128(x, y)
#define XOR(x,y) _mm_xor_si128(x,y)
#define OR(x,y) _mm_or_si128(x,y)
#define AND(x,y) _mm_and_si128(x,y)

#define ADD(x,y) _mm_add_epi32(x,y)
#define SHIFT_L(x,r) _mm_slli_epi32(x,r)
#define SHIFT_R(x,r) _mm_srli_epi32(x,r)

typedef struct LSH_ALIGNED_(32) {
	word msglen;
	LSH_ALIGNED_(32) __m128i lsh[4];
	LSH_ALIGNED_(32) __m128i buf[8];
	byte msg_buf[128];

}SIMD_LSH_INFO;
void SIMD_MsgExp(IN __m128i buf[8], OUT __m128i output[27][4]);
void SIMD_Compression(SIMD_LSH_INFO* info);
void upload_byte2SIMD(IN SIMD_LSH_INFO* info);
void SIMD_Mix_odd_with_permu(__m128i T[4], int Ns, __m128i output[4], __m128i sc[2]);
void SIMD_Mix_even_with_permu(__m128i T[4], int Ns, __m128i output[4], __m128i sc[2]);
void SIMD_LSH256_Process(IN byte* msg, IN word byte_msglen, OUT SIMD_LSH_INFO* info);
void SIMD_LSH256_Final(IN SIMD_LSH_INFO* info, OUT byte* output);
void SIMD_LSH256(IN const byte* msg, IN word msg_len, OUT byte* output);
