#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned char byte;
typedef unsigned int word;

#define IN 
#define OUT

#define ROTL(x, n)			(((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR(x, n)			(((x) >> (n)) | ((x) << (32 - (n))))
#define ENDIAN_CHANGE(X)	((ROTL((X),  8) & 0x00ff00ff) | (ROTL((X), 24) & 0xff00ff00))
#define IPAD 0x36
#define OPAD 0x5c

unsigned char getHex(unsigned char ch);
void convertStr2Byte(char* from, int size, unsigned char* to);
__int64 cpucycles();
int SIMD_PBKDF2_HMAC_LSH_256_ALL_Use_KISA(
	IN const byte* password,
	IN const word password_size,
	IN const byte* salt,
	IN const word salt_size,
	IN const word iteration_count,
	IN const word derkey_len,
	OUT		byte* derkey
);
