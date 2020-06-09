
#include <intrin.h>
#include <emmintrin.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <string.h>
#include "Type.h"
#include <stdint.h> 

typedef uint64_t lsh_u64;
typedef uint32_t lsh_u32;
typedef uint16_t lsh_u16;
typedef uint8_t  lsh_u8;
typedef uint32_t lsh_uint;
typedef lsh_u32 lsh_type;
typedef lsh_uint lsh_err;

#define LSH_TYPE_256_256				0x0000020
#define LSH_TYPE_256_224				0x000001C

#define LSH_TYPE_512_512				0x0010040
#define LSH_TYPE_512_384				0x0010030
#define LSH_TYPE_512_256				0x0010020
#define LSH_TYPE_512_224				0x001001C

#define LSH_TYPE_224					LSH_TYPE_256_224
#define LSH_TYPE_256					LSH_TYPE_256_256
#define LSH_TYPE_384					LSH_TYPE_512_384
#define LSH_TYPE_512					LSH_TYPE_512_512

/* LSH AlgType Macro */

#define LSH_IS_LSH256(lsh_type_val)				(((lsh_type_val)&0xf0000)==0)
#define LSH_IS_LSH512(lsh_type_val)				(((lsh_type_val)&0xf0000)==0x10000)

#define LSH_GET_SMALL_HASHBIT(lsh_type_val)		((lsh_type_val)>>24)
#define LSH_GET_HASHBYTE(lsh_type_val)			((lsh_type_val) & 0xffff)
#define LSH_GET_HASHBIT(lsh_type_val)			((LSH_GET_HASHBYTE(lsh_type_val)<<3)-LSH_GET_SMALL_HASHBIT(lsh_type_val))

#define LSH_MAKE_TYPE(is_lsh512, hash_bit_len)	(lsh_type)(((is_lsh512)?0x10000:0) \
												| (((hash_bit_len)&7)	?((((hash_bit_len)>>3) + 1) | ((8 - ((hash_bit_len)&7))<<24)) \
																		:((hash_bit_len)>>3)))

/* LSH Constants */

#define LSH256_MSG_BLK_BYTE_LEN			128
#define LSH256_MSG_BLK_BIT_LEN			1024
#define LSH256_CV_BYTE_LEN				64
#define LSH256_HASH_VAL_MAX_BYTE_LEN	32

#define LSH512_MSG_BLK_BYTE_LEN			256
#define LSH512_MSG_BLK_BIT_LEN			2048
#define LSH512_CV_BYTE_LEN				128
#define LSH512_HASH_VAL_MAX_BYTE_LEN	64

/* Error Code */

#define LSH_SUCCESS					0x0
#define LSH_ERR_NULL_PTR			0x2401
#define LSH_ERR_INVALID_ALGTYPE		0x2402
#define LSH_ERR_INVALID_DATABITLEN	0x2403
#define LSH_ERR_INVALID_STATE		0x2404

#define LSH_ALIGNED_(x) __declspec(align(x))


struct LSH_ALIGNED_(32) SIMD_LSH256_Context {
	LSH_ALIGNED_(16) lsh_type algtype;
	LSH_ALIGNED_(16) lsh_uint remain_databitlen;
	LSH_ALIGNED_(32) lsh_u32 cv_l[8];
	LSH_ALIGNED_(32) lsh_u32 cv_r[8];
	LSH_ALIGNED_(32) lsh_u8 last_block[LSH256_MSG_BLK_BYTE_LEN];
};


 /* -------------------------------------------------------- *
 *  LSH: parameters
 *  -------------------------------------------------------- */
#define MSG_BLK_WORD_LEN		32
#define CV_WORD_LEN				16
#define CONST_WORD_LEN			8
#define HASH_VAL_MAX_WORD_LEN	8

#define WORD_BIT_LEN			32

 /* -------------------------------------------------------- */

#define NUM_STEPS				26

#define ROT_EVEN_ALPHA			29
#define ROT_EVEN_BETA			1
#define ROT_ODD_ALPHA			5
#define ROT_ODD_BETA			17

/* -------------------------------------------------------- */

#define _LSH256_
#define _VER_256_BIT_REG_

/* -------------------------------------------------------- *
*  LSH: variables
*  -------------------------------------------------------- */


/* -------------------------------------------------------- *
* LSH: iv
* -------------------------------------------------------- */
const LSH_ALIGNED_(32) lsh_u32 C_g_IV224[CV_WORD_LEN] = {
	0x068608D3, 0x62D8F7A7, 0xD76652AB, 0x4C600A43, 0xBDC40AA8, 0x1ECA0B68, 0xDA1A89BE, 0x3147D354,
	0x707EB4F9, 0xF65B3862, 0x6B0B2ABE, 0x56B8EC0A, 0xCF237286, 0xEE0D1727, 0x33636595, 0x8BB8D05F,
};

const LSH_ALIGNED_(32) lsh_u32 C_g_IV256[CV_WORD_LEN] = {
	0x46a10f1f, 0xfddce486, 0xb41443a8, 0x198e6b9d, 0x3304388d, 0xb0f5a3c7, 0xb36061c4, 0x7adbd553,
	0x105d5378, 0x2f74de54, 0x5c2f2d95, 0xf2553fbe, 0x8051357a, 0x138668c8, 0x47aa4484, 0xe01afb41
};
typedef struct LSH_ALIGNED_(32) {
	LSH_ALIGNED_(16) lsh_type algtype;
	LSH_ALIGNED_(16) lsh_uint remain_databitlen;
	LSH_ALIGNED_(32) __m128i cv_l[2];				// left chaining variable
	LSH_ALIGNED_(32) __m128i cv_r[2];				// right chaining variable
	LSH_ALIGNED_(32) lsh_u8 last_block[LSH256_MSG_BLK_BYTE_LEN];
} LSH256SSE2_Context;


typedef struct LSH_ALIGNED_(32) {
	LSH_ALIGNED_(32) __m128i submsg_e_l[2];	/* even left sub-message */
	LSH_ALIGNED_(32) __m128i submsg_e_r[2];	/* even right sub-message */
	LSH_ALIGNED_(32) __m128i submsg_o_l[2];	/* odd left sub-message */
	LSH_ALIGNED_(32) __m128i submsg_o_r[2];	/* odd right sub-message */
} LSH256SSE2_internal;
/* -------------------------------------------------------- */

/* -------------------------------------------------------- */


const LSH_ALIGNED_(32) lsh_u32 C_g_StepConstants[CONST_WORD_LEN * NUM_STEPS] = {
	0x917caf90, 0x6c1b10a2, 0x6f352943, 0xcf778243, 0x2ceb7472, 0x29e96ff2, 0x8a9ba428, 0x2eeb2642,
	0x0e2c4021, 0x872bb30e, 0xa45e6cb2, 0x46f9c612, 0x185fe69e, 0x1359621b, 0x263fccb2, 0x1a116870,
	0x3a6c612f, 0xb2dec195, 0x02cb1f56, 0x40bfd858, 0x784684b6, 0x6cbb7d2e, 0x660c7ed8, 0x2b79d88a,
	0xa6cd9069, 0x91a05747, 0xcdea7558, 0x00983098, 0xbecb3b2e, 0x2838ab9a, 0x728b573e, 0xa55262b5,
	0x745dfa0f, 0x31f79ed8, 0xb85fce25, 0x98c8c898, 0x8a0669ec, 0x60e445c2, 0xfde295b0, 0xf7b5185a,
	0xd2580983, 0x29967709, 0x182df3dd, 0x61916130, 0x90705676, 0x452a0822, 0xe07846ad, 0xaccd7351,
	0x2a618d55, 0xc00d8032, 0x4621d0f5, 0xf2f29191, 0x00c6cd06, 0x6f322a67, 0x58bef48d, 0x7a40c4fd,
	0x8beee27f, 0xcd8db2f2, 0x67f2c63b, 0xe5842383, 0xc793d306, 0xa15c91d6, 0x17b381e5, 0xbb05c277,
	0x7ad1620a, 0x5b40a5bf, 0x5ab901a2, 0x69a7a768, 0x5b66d9cd, 0xfdee6877, 0xcb3566fc, 0xc0c83a32,
	0x4c336c84, 0x9be6651a, 0x13baa3fc, 0x114f0fd1, 0xc240a728, 0xec56e074, 0x009c63c7, 0x89026cf2,
	0x7f9ff0d0, 0x824b7fb5, 0xce5ea00f, 0x605ee0e2, 0x02e7cfea, 0x43375560, 0x9d002ac7, 0x8b6f5f7b,
	0x1f90c14f, 0xcdcb3537, 0x2cfeafdd, 0xbf3fc342, 0xeab7b9ec, 0x7a8cb5a3, 0x9d2af264, 0xfacedb06,
	0xb052106e, 0x99006d04, 0x2bae8d09, 0xff030601, 0xa271a6d6, 0x0742591d, 0xc81d5701, 0xc9a9e200,
	0x02627f1e, 0x996d719d, 0xda3b9634, 0x02090800, 0x14187d78, 0x499b7624, 0xe57458c9, 0x738be2c9,
	0x64e19d20, 0x06df0f36, 0x15d1cb0e, 0x0b110802, 0x2c95f58c, 0xe5119a6d, 0x59cd22ae, 0xff6eac3c,
	0x467ebd84, 0xe5ee453c, 0xe79cd923, 0x1c190a0d, 0xc28b81b8, 0xf6ac0852, 0x26efd107, 0x6e1ae93b,
	0xc53c41ca, 0xd4338221, 0x8475fd0a, 0x35231729, 0x4e0d3a7a, 0xa2b45b48, 0x16c0d82d, 0x890424a9,
	0x017e0c8f, 0x07b5a3f5, 0xfa73078e, 0x583a405e, 0x5b47b4c8, 0x570fa3ea, 0xd7990543, 0x8d28ce32,
	0x7f8a9b90, 0xbd5998fc, 0x6d7a9688, 0x927a9eb6, 0xa2fc7d23, 0x66b38e41, 0x709e491a, 0xb5f700bf,
	0x0a262c0f, 0x16f295b9, 0xe8111ef5, 0x0d195548, 0x9f79a0c5, 0x1a41cfa7, 0x0ee7638a, 0xacf7c074,
	0x30523b19, 0x09884ecf, 0xf93014dd, 0x266e9d55, 0x191a6664, 0x5c1176c1, 0xf64aed98, 0xa4b83520,
	0x828d5449, 0x91d71dd8, 0x2944f2d6, 0x950bf27b, 0x3380ca7d, 0x6d88381d, 0x4138868e, 0x5ced55c4,
	0x0fe19dcb, 0x68f4f669, 0x6e37c8ff, 0xa0fe6e10, 0xb44b47b0, 0xf5c0558a, 0x79bf14cf, 0x4a431a20,
	0xf17f68da, 0x5deb5fd1, 0xa600c86d, 0x9f6c7eb0, 0xff92f864, 0xb615e07f, 0x38d3e448, 0x8d5d3a6a,
	0x70e843cb, 0x494b312e, 0xa6c93613, 0x0beb2f4f, 0x928b5d63, 0xcbf66035, 0x0cb82c80, 0xea97a4f7,
	0x592c0f3b, 0x947c5f77, 0x6fff49b9, 0xf71a7e5a, 0x1de8c0f5, 0xc2569600, 0xc4e4ac8c, 0x823c9ce1
};

/* -------------------------------------------------------- */
// LSH: functions
/* -------------------------------------------------------- */
/* -------------------------------------------------------- */
// register functions macro
/* -------------------------------------------------------- */

#define LOAD(x) _mm_loadu_si128((__m128i*)x)
#define STORE(x,y) _mm_storeu_si128((__m128i*)x, y)
#define XOR(x,y) _mm_xor_si128(x,y)
#define OR(x,y) _mm_or_si128(x,y)
#define AND(x,y) _mm_and_si128(x,y)

#define ADD(x,y) _mm_add_epi32(x,y)
#define SHIFT_L(x,r) _mm_slli_epi32(x,r)
#define SHIFT_R(x,r) _mm_srli_epi32(x,r)

/* -------------------------------------------------------- */
// load a message block to register
/* -------------------------------------------------------- */

void C_load_blk(__m128i* dest, const void* src) {
	dest[0] = LOAD((const __m128i*)src);
	dest[1] = LOAD((const __m128i*)src + 1);
}

void C_store_blk(__m128i* dest, const __m128i* src) {
	STORE(dest, src[0]);
	STORE(dest + 1, src[1]);
}

void C_load_msg_blk(LSH256SSE2_internal* i_state, const lsh_u32* msgblk) {
	C_load_blk(i_state->submsg_e_l, msgblk + 0);
	C_load_blk(i_state->submsg_e_r, msgblk + 8);
	C_load_blk(i_state->submsg_o_l, msgblk + 16);
	C_load_blk(i_state->submsg_o_r, msgblk + 24);
}
void C_msg_exp_even(LSH256SSE2_internal* i_state) {
	i_state->submsg_e_l[0] = ADD(i_state->submsg_o_l[0], _mm_shuffle_epi32(i_state->submsg_e_l[0], 0x4b));
	i_state->submsg_e_l[1] = ADD(i_state->submsg_o_l[1], _mm_shuffle_epi32(i_state->submsg_e_l[1], 0x93));
	i_state->submsg_e_r[0] = ADD(i_state->submsg_o_r[0], _mm_shuffle_epi32(i_state->submsg_e_r[0], 0x4b));
	i_state->submsg_e_r[1] = ADD(i_state->submsg_o_r[1], _mm_shuffle_epi32(i_state->submsg_e_r[1], 0x93));
}
void C_msg_exp_odd(LSH256SSE2_internal* i_state) {
	i_state->submsg_o_l[0] = ADD(i_state->submsg_e_l[0], _mm_shuffle_epi32(i_state->submsg_o_l[0], 0x4b));
	i_state->submsg_o_l[1] = ADD(i_state->submsg_e_l[1], _mm_shuffle_epi32(i_state->submsg_o_l[1], 0x93));
	i_state->submsg_o_r[0] = ADD(i_state->submsg_e_r[0], _mm_shuffle_epi32(i_state->submsg_o_r[0], 0x4b));
	i_state->submsg_o_r[1] = ADD(i_state->submsg_e_r[1], _mm_shuffle_epi32(i_state->submsg_o_r[1], 0x93));
}
void C_load_sc(__m128i* const_v, lsh_uint i) {
	C_load_blk(const_v, C_g_StepConstants + i);
}
void C_msg_add_even(__m128i* cv_l, __m128i* cv_r, const LSH256SSE2_internal* i_state) {
	cv_l[0] = XOR(cv_l[0], i_state->submsg_e_l[0]);
	cv_r[0] = XOR(cv_r[0], i_state->submsg_e_r[0]);
	cv_l[1] = XOR(cv_l[1], i_state->submsg_e_l[1]);
	cv_r[1] = XOR(cv_r[1], i_state->submsg_e_r[1]);
}
void C_msg_add_odd(__m128i* cv_l, __m128i* cv_r, const LSH256SSE2_internal* i_state) {
	cv_l[0] = XOR(cv_l[0], i_state->submsg_o_l[0]);
	cv_r[0] = XOR(cv_r[0], i_state->submsg_o_r[0]);
	cv_l[1] = XOR(cv_l[1], i_state->submsg_o_l[1]);
	cv_r[1] = XOR(cv_r[1], i_state->submsg_o_r[1]);
}
void C_add_blk(__m128i* cv_l, const __m128i* cv_r) {
	cv_l[0] = ADD(cv_l[0], cv_r[0]);
	cv_l[1] = ADD(cv_l[1], cv_r[1]);
}
void C_rotate_blk_even_alpha(__m128i* cv) {
	cv[0] = OR(SHIFT_L(cv[0], ROT_EVEN_ALPHA), SHIFT_R(cv[0], WORD_BIT_LEN - ROT_EVEN_ALPHA));
	cv[1] = OR(SHIFT_L(cv[1], ROT_EVEN_ALPHA), SHIFT_R(cv[1], WORD_BIT_LEN - ROT_EVEN_ALPHA));
}
void C_rotate_blk_even_beta(__m128i* cv) {
	cv[0] = OR(SHIFT_L(cv[0], ROT_EVEN_BETA), SHIFT_R(cv[0], WORD_BIT_LEN - ROT_EVEN_BETA));
	cv[1] = OR(SHIFT_L(cv[1], ROT_EVEN_BETA), SHIFT_R(cv[1], WORD_BIT_LEN - ROT_EVEN_BETA));
}
void C_rotate_blk_odd_alpha(__m128i* cv) {
	cv[0] = OR(SHIFT_L(cv[0], ROT_ODD_ALPHA), SHIFT_R(cv[0], WORD_BIT_LEN - ROT_ODD_ALPHA));
	cv[1] = OR(SHIFT_L(cv[1], ROT_ODD_ALPHA), SHIFT_R(cv[1], WORD_BIT_LEN - ROT_ODD_ALPHA));
}
void C_rotate_blk_odd_beta(__m128i* cv) {
	cv[0] = OR(SHIFT_L(cv[0], ROT_ODD_BETA), SHIFT_R(cv[0], WORD_BIT_LEN - ROT_ODD_BETA));
	cv[1] = OR(SHIFT_L(cv[1], ROT_ODD_BETA), SHIFT_R(cv[1], WORD_BIT_LEN - ROT_ODD_BETA));
}
void C_xor_with_const(__m128i* cv_l, const __m128i* const_v) {
	cv_l[0] = XOR(cv_l[0], const_v[0]);
	cv_l[1] = XOR(cv_l[1], const_v[1]);
}
void C_rotate_msg_gamma(__m128i* cv_r) {
	__m128i temp;

	temp = AND(cv_r[0], _mm_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0x0));
	cv_r[0] = AND(cv_r[0], _mm_set_epi32(0x0, 0x0, 0x0, 0xffffffff));

	temp = XOR(SHIFT_L(temp, 8), SHIFT_R(temp, 24));
	cv_r[0] = XOR(cv_r[0], temp);

	temp = AND(cv_r[0], _mm_set_epi32(0xffffffff, 0xffffffff, 0x0, 0x0));
	cv_r[0] = AND(cv_r[0], _mm_set_epi32(0x0, 0x0, 0xffffffff, 0xffffffff));

	temp = XOR(SHIFT_L(temp, 8), SHIFT_R(temp, 24));
	cv_r[0] = XOR(cv_r[0], temp);

	temp = AND(cv_r[0], _mm_set_epi32(0xffffffff, 0x0, 0x0, 0x0));
	cv_r[0] = AND(cv_r[0], _mm_set_epi32(0x0, 0xffffffff, 0xffffffff, 0xffffffff));

	temp = XOR(SHIFT_L(temp, 8), SHIFT_R(temp, 24));
	cv_r[0] = XOR(cv_r[0], temp);
	////////////////////////////////////////////////////////////////////////////////
	temp = AND(cv_r[1], _mm_set_epi32(0x0, 0xffffffff, 0xffffffff, 0xffffffff));
	cv_r[1] = AND(cv_r[1], _mm_set_epi32(0xffffffff, 0x0, 0x0, 0x0));

	temp = XOR(SHIFT_L(temp, 8), SHIFT_R(temp, 24));
	cv_r[1] = XOR(cv_r[1], temp);

	temp = AND(cv_r[1], _mm_set_epi32(0x0, 0x0, 0xffffffff, 0xffffffff));
	cv_r[1] = AND(cv_r[1], _mm_set_epi32(0xffffffff, 0xffffffff, 0x0, 0x0));

	temp = XOR(SHIFT_L(temp, 8), SHIFT_R(temp, 24));
	cv_r[1] = XOR(cv_r[1], temp);

	temp = AND(cv_r[1], _mm_set_epi32(0x0, 0x0, 0x0, 0xffffffff));
	cv_r[1] = AND(cv_r[1], _mm_set_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0x0));

	temp = XOR(SHIFT_L(temp, 8), SHIFT_R(temp, 24));
	cv_r[1] = XOR(cv_r[1], temp);
}
void C_word_perm(__m128i* cv_l, __m128i* cv_r) {
	__m128i temp;
	cv_l[0] = _mm_shuffle_epi32(cv_l[0], 0xd2);
	cv_l[1] = _mm_shuffle_epi32(cv_l[1], 0xd2);
	cv_r[0] = _mm_shuffle_epi32(cv_r[0], 0x6c);
	cv_r[1] = _mm_shuffle_epi32(cv_r[1], 0x6c);
	temp = cv_l[0];
	cv_l[0] = cv_l[1];
	cv_l[1] = cv_r[1];
	cv_r[1] = cv_r[0];
	cv_r[0] = temp;
};

/* -------------------------------------------------------- */
// step function
/* -------------------------------------------------------- */

void C_mix_even(__m128i* cv_l, __m128i* cv_r, const __m128i* const_v) {
	C_add_blk(cv_l, cv_r);
	C_rotate_blk_even_alpha(cv_l);
	C_xor_with_const(cv_l, const_v);
	C_add_blk(cv_r, cv_l);
	C_rotate_blk_even_beta(cv_r);
	C_add_blk(cv_l, cv_r);
	C_rotate_msg_gamma(cv_r);
}

void C_mix_odd(__m128i* cv_l, __m128i* cv_r, const __m128i* const_v) {
	C_add_blk(cv_l, cv_r);
	C_rotate_blk_odd_alpha(cv_l);
	C_xor_with_const(cv_l, const_v);
	C_add_blk(cv_r, cv_l);
	C_rotate_blk_odd_beta(cv_r);
	C_add_blk(cv_l, cv_r);
	C_rotate_msg_gamma(cv_r);
}

/* -------------------------------------------------------- */
// compression function
/* -------------------------------------------------------- */

void C_compress(__m128i* cv_l, __m128i* cv_r, const lsh_u32 pdMsgBlk[MSG_BLK_WORD_LEN])
{
	__m128i const_v[2];			// step function constant
	LSH256SSE2_internal i_state[1];
	int i;

	C_load_msg_blk(i_state, pdMsgBlk);

	C_msg_add_even(cv_l, cv_r, i_state);
	C_load_sc(const_v, 0);
	C_mix_even(cv_l, cv_r, const_v);
	C_word_perm(cv_l, cv_r);

	C_msg_add_odd(cv_l, cv_r, i_state);
	C_load_sc(const_v, 8);
	C_mix_odd(cv_l, cv_r, const_v);
	C_word_perm(cv_l, cv_r);

	for (i = 1; i < NUM_STEPS / 2; i++) {


		C_msg_exp_even(i_state);


		C_msg_add_even(cv_l, cv_r, i_state);
		C_load_sc(const_v, 16 * i);
		C_mix_even(cv_l, cv_r, const_v);
		C_word_perm(cv_l, cv_r);

		C_msg_exp_odd(i_state);
		C_msg_add_odd(cv_l, cv_r, i_state);
		C_load_sc(const_v, 16 * i + 8);
		C_mix_odd(cv_l, cv_r, const_v);
		C_word_perm(cv_l, cv_r);
	}
	C_msg_exp_even(i_state);
	C_msg_add_even(cv_l, cv_r, i_state);

}


/* -------------------------------------------------------- */

void C_init224(LSH256SSE2_Context* state)
{
	C_load_blk(state->cv_l, C_g_IV224);
	C_load_blk(state->cv_r, C_g_IV224 + 8);
}

void C_init256(LSH256SSE2_Context* state)
{
	C_load_blk(state->cv_l, C_g_IV256);
	C_load_blk(state->cv_r, C_g_IV256 + 8);
}

/* -------------------------------------------------------- */

void C_fin(__m128i* cv_l, const __m128i* cv_r)
{
	cv_l[0] = XOR(cv_l[0], cv_r[0]);
	cv_l[1] = XOR(cv_l[1], cv_r[1]);
}

/* -------------------------------------------------------- */

void C_get_hash(__m128i* cv_l, lsh_u8* pbHashVal, const lsh_type algtype)
{
	lsh_u8 hash_val[LSH256_HASH_VAL_MAX_BYTE_LEN] = { 0x0, };
	lsh_uint hash_val_byte_len = LSH_GET_HASHBYTE(algtype);
	lsh_uint hash_val_bit_len = LSH_GET_SMALL_HASHBIT(algtype);

	STORE(hash_val, cv_l[0]);
	STORE((hash_val + 16), cv_l[1]);
	memcpy(pbHashVal, hash_val, sizeof(lsh_u8) * hash_val_byte_len);
	if (hash_val_bit_len) {
		pbHashVal[hash_val_byte_len - 1] &= (((lsh_u8)0xff) << hash_val_bit_len);
	}
}

/* -------------------------------------------------------- */

lsh_err C_lsh256_sse2_init(struct LSH256_Context* _ctx, const lsh_type algtype) {

	LSH256SSE2_Context* ctx = (LSH256SSE2_Context*)_ctx;
	__m128i cv_l[2];
	__m128i cv_r[2];
	__m128i const_v[2];
	lsh_uint i;

	if (ctx == NULL) {
		return LSH_ERR_NULL_PTR;
	}

	ctx->algtype = algtype;
	ctx->remain_databitlen = 0;

	C_init256(ctx);


	cv_l[0] = _mm_set_epi32(0, 0, LSH_GET_HASHBIT(algtype), LSH256_HASH_VAL_MAX_BYTE_LEN);
	cv_l[1] = _mm_setzero_si128();
	cv_r[0] = _mm_setzero_si128();
	cv_r[1] = _mm_setzero_si128();


	for (i = 0; i < NUM_STEPS / 2; i++)
	{
		//Mix
		C_load_sc(const_v, i * 16);
		C_mix_even(cv_l, cv_r, const_v);
		C_word_perm(cv_l, cv_r);

		C_load_sc(const_v, i * 16 + 8);
		C_mix_odd(cv_l, cv_r, const_v);
		C_word_perm(cv_l, cv_r);
	}

	C_store_blk(ctx->cv_l, cv_l);
	C_store_blk(ctx->cv_r, cv_r);


	return LSH_SUCCESS;
}


lsh_err C_lsh256_sse2_update(struct LSH256_Context* _ctx, const lsh_u8* data, size_t databitlen) {
	__m128i cv_l[2];
	__m128i cv_r[2];
	size_t databytelen = databitlen >> 3;
	lsh_uint pos2 = databitlen & 0x7;

	LSH256SSE2_Context* ctx = (LSH256SSE2_Context*)_ctx;
	lsh_uint remain_msg_byte;
	lsh_uint remain_msg_bit;

	if (ctx == NULL || data == NULL) {
		return LSH_ERR_NULL_PTR;
	}
	if (ctx->algtype == 0 || LSH_GET_HASHBYTE(ctx->algtype) > LSH256_HASH_VAL_MAX_BYTE_LEN) {
		return LSH_ERR_INVALID_STATE;
	}
	if (databitlen == 0) {
		return LSH_SUCCESS;
	}
	remain_msg_byte = ctx->remain_databitlen >> 3;
	remain_msg_bit = ctx->remain_databitlen & 7;
	if (remain_msg_byte >= LSH256_MSG_BLK_BYTE_LEN) {
		printf("%d\n", remain_msg_byte);
		return LSH_ERR_INVALID_STATE;
	}
	if (remain_msg_bit > 0) {
		return LSH_ERR_INVALID_DATABITLEN;
	}

	if (databytelen + remain_msg_byte < LSH256_MSG_BLK_BYTE_LEN) {
		memcpy(ctx->last_block + remain_msg_byte, data, databytelen);
		ctx->remain_databitlen += (lsh_uint)databitlen;
		remain_msg_byte += (lsh_uint)databytelen;
		if (pos2) {
			ctx->last_block[remain_msg_byte] = data[databytelen] & ((0xff >> pos2) ^ 0xff);
		}
		return LSH_SUCCESS;
	}

	C_load_blk(cv_l, ctx->cv_l);
	C_load_blk(cv_r, ctx->cv_r);

	if (remain_msg_byte > 0) {
		lsh_uint more_byte = LSH256_MSG_BLK_BYTE_LEN - remain_msg_byte;
		memcpy(ctx->last_block + remain_msg_byte, data, more_byte);
		C_compress(cv_l, cv_r, (lsh_u32*)ctx->last_block);
		data += more_byte;
		databytelen -= more_byte;
		remain_msg_byte = 0;
		ctx->remain_databitlen = 0;
	}



	unsigned long long cycle2 = 0;
	unsigned long long cycle1 = 0;
	while (databytelen >= LSH256_MSG_BLK_BYTE_LEN)
	{
		C_compress(cv_l, cv_r, (lsh_u32*)data);
		data += LSH256_MSG_BLK_BYTE_LEN;
		databytelen -= LSH256_MSG_BLK_BYTE_LEN;
	}

	C_store_blk(ctx->cv_l, cv_l);
	C_store_blk(ctx->cv_r, cv_r);

	if (databytelen > 0) {
		memcpy(ctx->last_block, data, databytelen);
		ctx->remain_databitlen = (lsh_uint)(databytelen << 3);
	}

	if (pos2) {
		ctx->last_block[databytelen] = data[databytelen] & ((0xff >> pos2) ^ 0xff);
		ctx->remain_databitlen += pos2;
	}

	return LSH_SUCCESS;
}

lsh_err C_lsh256_sse2_final(struct SIMD_LSH256_Context* _ctx, lsh_u8* hashval) {
	__m128i cv_l[2];
	__m128i cv_r[2];
	LSH256SSE2_Context* ctx = (LSH256SSE2_Context*)_ctx;
	lsh_uint remain_msg_byte;
	lsh_uint remain_msg_bit;

	if (ctx == NULL || hashval == NULL) {
		return LSH_ERR_NULL_PTR;
	}
	if (ctx->algtype == 0 || LSH_GET_HASHBYTE(ctx->algtype) > LSH256_HASH_VAL_MAX_BYTE_LEN) {
		return LSH_ERR_INVALID_STATE;
	}

	remain_msg_byte = ctx->remain_databitlen >> 3;
	remain_msg_bit = ctx->remain_databitlen & 7;

	if (remain_msg_byte >= LSH256_MSG_BLK_BYTE_LEN) {
		return LSH_ERR_INVALID_STATE;
	}

	if (remain_msg_bit) {
		ctx->last_block[remain_msg_byte] |= (0x1 << (7 - remain_msg_bit));
	}
	else {
		ctx->last_block[remain_msg_byte] = 0x80;
	}
	memset(ctx->last_block + remain_msg_byte + 1, 0, LSH256_MSG_BLK_BYTE_LEN - remain_msg_byte - 1);

	C_load_blk(cv_l, ctx->cv_l);
	C_load_blk(cv_r, ctx->cv_r);

	C_compress(cv_l, cv_r, (lsh_u32*)ctx->last_block);

	C_fin(cv_l, cv_r);
	C_get_hash(cv_l, hashval, ctx->algtype);

	memset(ctx, 0, sizeof(struct SIMD_LSH256_Context));

	return LSH_SUCCESS;
}

lsh_err C_lsh256_sse2_digest(const lsh_type algtype, const lsh_u8* data, size_t databitlen, lsh_u8* hashval) {
	lsh_err result;
	struct SIMD_LSH256_Context ctx;

	unsigned long long cycle1 = 0;
	unsigned long long cycle2 = 0;

	//cycle1 = cpucycles();
	result = C_lsh256_sse2_init(&ctx, algtype);
	//	cycle2 = cpucycles();
		//printf("%10lld\n", cycle2 - cycle1);
		//cycle1 = cpucycles();
	result = C_lsh256_sse2_update(&ctx, data, databitlen);
	//cycle2 = cpucycles();
	//printf("%10lld\n", cycle2 - cycle1);
	//cycle1 = cpucycles();
	result = C_lsh256_sse2_final(&ctx, hashval);
	//	cycle2 = cpucycles();
	//	printf("%10lld\n", cycle2 - cycle1);
	return result;
}


void C_Change_compress(const byte pdMsgBlk[32], __m128i state[4])
{
	__m128i const_v[2];			// step function constant
	LSH256SSE2_internal i_state[1];
	int i;

	__m128i cv_l[2] = { 0, };
	__m128i cv_r[2] = { 0, };
	cv_l[0] = LOAD((const __m128i*)C_g_IV256);
	cv_l[1] = LOAD((const __m128i*)C_g_IV256 + 1);
	cv_r[0] = LOAD((const __m128i*)C_g_IV256 + 2);
	cv_r[1] = LOAD((const __m128i*)C_g_IV256 + 3);

	C_load_msg_blk(i_state, pdMsgBlk);

	C_msg_add_even(cv_l, cv_r, i_state);
	C_load_sc(const_v, 0);
	C_mix_even(cv_l, cv_r, const_v);
	C_word_perm(cv_l, cv_r);

	C_msg_add_odd(cv_l, cv_r, i_state);
	C_load_sc(const_v, 8);
	C_mix_odd(cv_l, cv_r, const_v);
	C_word_perm(cv_l, cv_r);

	for (i = 1; i < NUM_STEPS / 2; i++) {


		C_msg_exp_even(i_state);
		C_msg_add_even(cv_l, cv_r, i_state);
		C_load_sc(const_v, 16 * i);
		C_mix_even(cv_l, cv_r, const_v);
		C_word_perm(cv_l, cv_r);

		C_msg_exp_odd(i_state);
		C_msg_add_odd(cv_l, cv_r, i_state);
		C_load_sc(const_v, 16 * i + 8);
		C_mix_odd(cv_l, cv_r, const_v);
		C_word_perm(cv_l, cv_r);
	}
	C_msg_exp_even(i_state);
	C_msg_add_even(cv_l, cv_r, i_state);
	state[0] = cv_l[0];
	state[1] = cv_l[1];
	state[2] = cv_r[0];
	state[3] = cv_r[1];


}

void SIMD_ALL_precompute_Use_KISA(
	IN const byte* password,
	IN const word password_size,
	OUT  __m128i IPAD_state[4],
	OUT  __m128i OPAD_state[4]
)
{


	byte K1[128] = { 0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36, 0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36,0x36 };
	byte K2[128] = { 0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c,0x5c };
	byte K0[128] = { 0, };
	int updatekeylen = 0;
	int cnt = 0;

	if (password_size > 128)
	{
		C_lsh256_sse2_digest(32, password, password_size << 3, K0);
		updatekeylen = 32;
		for (cnt = 0; cnt <32; cnt++) {
			K1[cnt] ^= K0[cnt];
			K2[cnt] ^= K0[cnt];
		}
	}
	else
	{

		for (cnt = 0; cnt < password_size; cnt++) {
			K1[cnt] ^= password[cnt];
			K2[cnt] ^= password[cnt];
		}
	}

	
	C_Change_compress(K1, IPAD_state);
	C_Change_compress(K2, OPAD_state);

}

lsh_err pre_lsh256_sse2_init(struct LSH256_Context* _ctx, const lsh_type algtype, __m128i State[4]) {

	LSH256SSE2_Context* ctx = (LSH256SSE2_Context*)_ctx;
	

	ctx->algtype = algtype;
	ctx->remain_databitlen = 0;

	ctx->cv_l[0] = State[0];
	ctx->cv_l[1] = State[1];
	ctx->cv_r[0] = State[2];
	ctx->cv_r[1] = State[3];

	return LSH_SUCCESS;
}

int SIMD_PBKDF2_HMAC_LSH_256_ALL_Use_KISA(
	IN const byte* password,
	IN const word password_size,
	IN const byte* salt,
	IN const word salt_size,
	IN const word iteration_count,
	IN const word derkey_len,
	OUT		byte* derkey
)
{
	byte K0[128] = { 0, };
	byte K1[128] = { 0, };
	byte K2[128] = { 0, };
	byte FirstOut[32] = { 0, };
	byte Finalout[32] = { 0, };
	byte Buffer[32] = { 0, };
	byte salt_value[1000] = { 0, };
	__declspec(align(32)) __m128i IPAD_state[4] = { 0, };
	__m128i OPAD_state[4];

#if 1
	struct SIMD_LSH256_Context ctx;
	int Q = 0;
	int R = 0;
	int iter_cnt = 0;
	int updatekeylen = 0;
	int j = 0;
	int i = 1;
	int index = 0;
	int result = 0;
	Q = derkey_len >> 5;
	R = derkey_len % 32;

	SIMD_ALL_precompute_Use_KISA(password, password_size, IPAD_state, OPAD_state);

	memcpy(salt_value, salt, salt_size);

	i = 1;
	while ((Q--) > 0)
	{

		salt_value[salt_size] = (i >> 24) & 0xff;
		salt_value[salt_size + 1] = (i >> 16) & 0xff;
		salt_value[salt_size + 2] = (i >> 8) & 0xff;
		salt_value[salt_size + 3] = (i) & 0xff;
		i = i + 1;

		//K1
		pre_lsh256_sse2_init(&ctx, 32, IPAD_state);
		result = C_lsh256_sse2_update(&ctx, salt_value, (salt_size+4)<<3);
		C_lsh256_sse2_final(&ctx, FirstOut);
		

		//K2
		pre_lsh256_sse2_init(&ctx, 32, OPAD_state);
		C_lsh256_sse2_update(&ctx,FirstOut, 256);
		C_lsh256_sse2_final(&ctx, Finalout);
		memcpy(Buffer, Finalout, 32);


		for (iter_cnt = 1; iter_cnt < iteration_count; iter_cnt++)
		{

			pre_lsh256_sse2_init(&ctx, 32, IPAD_state);
			result = C_lsh256_sse2_update(&ctx, Finalout, 256);
			C_lsh256_sse2_final(&ctx, FirstOut);
			
			pre_lsh256_sse2_init(&ctx, 32, OPAD_state);
			C_lsh256_sse2_update(&ctx, FirstOut, 256);
			C_lsh256_sse2_final(&ctx, Finalout);
			
			for (j = 0; j < 32; j++)
				Buffer[j] ^= Finalout[j];

		}
		for (j = 0; j < 32; j++, index++)
			derkey[index] = Buffer[j];
	}

	if (Q == 0)
		return 1;
	else
	{
		salt_value[salt_size] = (i >> 24) & 0xff;
		salt_value[salt_size + 1] = (i >> 16) & 0xff;
		salt_value[salt_size + 2] = (i >> 8) & 0xff;
		salt_value[salt_size + 3] = (i) & 0xff;
		i = i + 1;

		//K1
		pre_lsh256_sse2_init(&ctx, 32, IPAD_state);
		C_lsh256_sse2_update(&ctx, salt_value, (salt_size + 4) << 3);
		C_lsh256_sse2_final(&ctx, FirstOut);

		//K2
		pre_lsh256_sse2_init(&ctx, 32, OPAD_state);
		C_lsh256_sse2_update(&ctx,FirstOut, 256);
		C_lsh256_sse2_final(&ctx, Finalout);
		memcpy(Finalout, Buffer, 32);
		for (iter_cnt = 1; iter_cnt < iteration_count; iter_cnt++)
		{

			pre_lsh256_sse2_init(&ctx, 32, IPAD_state);
			C_lsh256_sse2_update(&ctx, Finalout, 256);
			C_lsh256_sse2_final(&ctx, FirstOut);


			pre_lsh256_sse2_init(&ctx, 32, OPAD_state);
			C_lsh256_sse2_update(&ctx, FirstOut, 256);
			C_lsh256_sse2_final(&ctx, Finalout);

			for (j = 0; j < 32; j++)
				Buffer[j] ^= Finalout[j];

		}
		for (j = 0; j < R; j++, index++)
			derkey[index] = Buffer[j];
	}
#endif
}