#pragma once
#include "Type.h"

#define loadLE32(v)			((v))

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

/* LSH AlgType Macro */

#define LSH_IS_LSH256(lsh_type_val)				(((lsh_type_val)&0xf0000)==0)
#define LSH_IS_LSH512(lsh_type_val)				(((lsh_type_val)&0xf0000)==0x10000)

#define LSH_GET_SMALL_HASHBIT(lsh_type_val)		((lsh_type_val)>>24)
#define LSH_GET_HASHBYTE(lsh_type_val)			((lsh_type_val) & 0xffff)
#define LSH_GET_HASHBIT(lsh_type_val)			((LSH_GET_HASHBYTE(lsh_type_val)<<3)-LSH_GET_SMALL_HASHBIT(lsh_type_val))

#define LSH_MAKE_TYPE(is_lsh512, hash_bit_len)	(lsh_type)(((is_lsh512)?0x10000:0) \
												| (((hash_bit_len)&7)	?((((hash_bit_len)>>3) + 1) | ((8 - ((hash_bit_len)&7))<<24)) \
																		:((hash_bit_len)>>3)))

/* LSH AlgType */

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

/* -------------------------------------------------------- *
* LSH: parameters
* -------------------------------------------------------- */
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

struct LSH256_internal {
	word submsg_e_l[CV_WORD_LEN / 2];	/* even left sub-message */
	word submsg_e_r[CV_WORD_LEN / 2];	/* even right sub-message */
	word submsg_o_l[CV_WORD_LEN / 2];	/* odd left sub-message */
	word submsg_o_r[CV_WORD_LEN / 2];	/* odd right sub-message */
};

/**
 * LSH256 내부 상태를 저장하기 위한 구조체
 */
struct LSH256_Context {
	word algtype;
	word remain_databitlen;
	word cv_l[8];
	word cv_r[8];
	byte last_block[LSH256_MSG_BLK_BYTE_LEN];
};

word lsh256_digest(const word algtype, const byte* data, size_t databitlen, byte* hashval);