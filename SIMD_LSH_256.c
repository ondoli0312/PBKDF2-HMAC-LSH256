#include "Type.h"
#include "LSH256.h"


const LSH_ALIGNED_(32) word SIMD_IV[16] = {
	0x46a10f1f, 0xfddce486, 0xb41443a8, 0x198e6b9d, 0x3304388d, 0xb0f5a3c7, 0xb36061c4, 0x7adbd553,
	0x105d5378, 0x2f74de54, 0x5c2f2d95, 0xf2553fbe, 0x8051357a, 0x138668c8, 0x47aa4484, 0xe01afb41
};

const LSH_ALIGNED_(32) word SIMD_SC[8 * 26] = {
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



void SIMD_MsgExp(IN __m128i buf[8], OUT __m128i output[27][4]) {

	_mm_store_si128(&output[0][0], buf[0]);
	_mm_store_si128(&output[0][1], buf[1]);
	_mm_store_si128(&output[0][2], buf[2]);
	_mm_store_si128(&output[0][3], buf[3]);

	_mm_store_si128(&output[1][0], buf[4]);
	_mm_store_si128(&output[1][1], buf[5]);
	_mm_store_si128(&output[1][2], buf[6]);
	_mm_store_si128(&output[1][3], buf[7]);

	output[2][0] = ADD(_mm_shuffle_epi32(output[0][0], 0x4b), output[1][0]);
	output[2][1] = ADD(_mm_shuffle_epi32(output[0][1], 0x93), output[1][1]);
	output[2][2] = ADD(_mm_shuffle_epi32(output[0][2], 0x4b), output[1][2]);
	output[2][3] = ADD(_mm_shuffle_epi32(output[0][3], 0x93), output[1][3]);

	output[3][0] = ADD(_mm_shuffle_epi32(output[1][0], 0x4b), output[2][0]);
	output[3][1] = ADD(_mm_shuffle_epi32(output[1][1], 0x93), output[2][1]);
	output[3][2] = ADD(_mm_shuffle_epi32(output[1][2], 0x4b), output[2][2]);
	output[3][3] = ADD(_mm_shuffle_epi32(output[1][3], 0x93), output[2][3]);

	output[4][0] = ADD(_mm_shuffle_epi32(output[2][0], 0x4b), output[3][0]);
	output[4][1] = ADD(_mm_shuffle_epi32(output[2][1], 0x93), output[3][1]);
	output[4][2] = ADD(_mm_shuffle_epi32(output[2][2], 0x4b), output[3][2]);
	output[4][3] = ADD(_mm_shuffle_epi32(output[2][3], 0x93), output[3][3]);

	output[5][0] = ADD(_mm_shuffle_epi32(output[3][0], 0x4b), output[4][0]);
	output[5][1] = ADD(_mm_shuffle_epi32(output[3][1], 0x93), output[4][1]);
	output[5][2] = ADD(_mm_shuffle_epi32(output[3][2], 0x4b), output[4][2]);
	output[5][3] = ADD(_mm_shuffle_epi32(output[3][3], 0x93), output[4][3]);

	output[6][0] = ADD(_mm_shuffle_epi32(output[4][0], 0x4b), output[5][0]);
	output[6][1] = ADD(_mm_shuffle_epi32(output[4][1], 0x93), output[5][1]);
	output[6][2] = ADD(_mm_shuffle_epi32(output[4][2], 0x4b), output[5][2]);
	output[6][3] = ADD(_mm_shuffle_epi32(output[4][3], 0x93), output[5][3]);

	output[7][0] = ADD(_mm_shuffle_epi32(output[5][0], 0x4b), output[6][0]);
	output[7][1] = ADD(_mm_shuffle_epi32(output[5][1], 0x93), output[6][1]);
	output[7][2] = ADD(_mm_shuffle_epi32(output[5][2], 0x4b), output[6][2]);
	output[7][3] = ADD(_mm_shuffle_epi32(output[5][3], 0x93), output[6][3]);

	output[8][0] = ADD(_mm_shuffle_epi32(output[6][0], 0x4b), output[7][0]);
	output[8][1] = ADD(_mm_shuffle_epi32(output[6][1], 0x93), output[7][1]);
	output[8][2] = ADD(_mm_shuffle_epi32(output[6][2], 0x4b), output[7][2]);
	output[8][3] = ADD(_mm_shuffle_epi32(output[6][3], 0x93), output[7][3]);

	output[9][0] = ADD(_mm_shuffle_epi32(output[7][0], 0x4b), output[8][0]);
	output[9][1] = ADD(_mm_shuffle_epi32(output[7][1], 0x93), output[8][1]);
	output[9][2] = ADD(_mm_shuffle_epi32(output[7][2], 0x4b), output[8][2]);
	output[9][3] = ADD(_mm_shuffle_epi32(output[7][3], 0x93), output[8][3]);

	output[10][0] = ADD(_mm_shuffle_epi32(output[8][0], 0x4b), output[9][0]);
	output[10][1] = ADD(_mm_shuffle_epi32(output[8][1], 0x93), output[9][1]);
	output[10][2] = ADD(_mm_shuffle_epi32(output[8][2], 0x4b), output[9][2]);
	output[10][3] = ADD(_mm_shuffle_epi32(output[8][3], 0x93), output[9][3]);

	output[11][0] = ADD(_mm_shuffle_epi32(output[9][0], 0x4b), output[10][0]);
	output[11][1] = ADD(_mm_shuffle_epi32(output[9][1], 0x93), output[10][1]);
	output[11][2] = ADD(_mm_shuffle_epi32(output[9][2], 0x4b), output[10][2]);
	output[11][3] = ADD(_mm_shuffle_epi32(output[9][3], 0x93), output[10][3]);

	output[12][0] = ADD(_mm_shuffle_epi32(output[10][0], 0x4b), output[11][0]);
	output[12][1] = ADD(_mm_shuffle_epi32(output[10][1], 0x93), output[11][1]);
	output[12][2] = ADD(_mm_shuffle_epi32(output[10][2], 0x4b), output[11][2]);
	output[12][3] = ADD(_mm_shuffle_epi32(output[10][3], 0x93), output[11][3]);

	output[13][0] = ADD(_mm_shuffle_epi32(output[11][0], 0x4b), output[12][0]);
	output[13][1] = ADD(_mm_shuffle_epi32(output[11][1], 0x93), output[12][1]);
	output[13][2] = ADD(_mm_shuffle_epi32(output[11][2], 0x4b), output[12][2]);
	output[13][3] = ADD(_mm_shuffle_epi32(output[11][3], 0x93), output[12][3]);

	output[14][0] = ADD(_mm_shuffle_epi32(output[12][0], 0x4b), output[13][0]);
	output[14][1] = ADD(_mm_shuffle_epi32(output[12][1], 0x93), output[13][1]);
	output[14][2] = ADD(_mm_shuffle_epi32(output[12][2], 0x4b), output[13][2]);
	output[14][3] = ADD(_mm_shuffle_epi32(output[12][3], 0x93), output[13][3]);

	output[15][0] = ADD(_mm_shuffle_epi32(output[13][0], 0x4b), output[14][0]);
	output[15][1] = ADD(_mm_shuffle_epi32(output[13][1], 0x93), output[14][1]);
	output[15][2] = ADD(_mm_shuffle_epi32(output[13][2], 0x4b), output[14][2]);
	output[15][3] = ADD(_mm_shuffle_epi32(output[13][3], 0x93), output[14][3]);

	output[16][0] = ADD(_mm_shuffle_epi32(output[14][0], 0x4b), output[15][0]);
	output[16][1] = ADD(_mm_shuffle_epi32(output[14][1], 0x93), output[15][1]);
	output[16][2] = ADD(_mm_shuffle_epi32(output[14][2], 0x4b), output[15][2]);
	output[16][3] = ADD(_mm_shuffle_epi32(output[14][3], 0x93), output[15][3]);

	output[17][0] = ADD(_mm_shuffle_epi32(output[15][0], 0x4b), output[16][0]);
	output[17][1] = ADD(_mm_shuffle_epi32(output[15][1], 0x93), output[16][1]);
	output[17][2] = ADD(_mm_shuffle_epi32(output[15][2], 0x4b), output[16][2]);
	output[17][3] = ADD(_mm_shuffle_epi32(output[15][3], 0x93), output[16][3]);

	output[18][0] = ADD(_mm_shuffle_epi32(output[16][0], 0x4b), output[17][0]);
	output[18][1] = ADD(_mm_shuffle_epi32(output[16][1], 0x93), output[17][1]);
	output[18][2] = ADD(_mm_shuffle_epi32(output[16][2], 0x4b), output[17][2]);
	output[18][3] = ADD(_mm_shuffle_epi32(output[16][3], 0x93), output[17][3]);

	output[19][0] = ADD(_mm_shuffle_epi32(output[17][0], 0x4b), output[18][0]);
	output[19][1] = ADD(_mm_shuffle_epi32(output[17][1], 0x93), output[18][1]);
	output[19][2] = ADD(_mm_shuffle_epi32(output[17][2], 0x4b), output[18][2]);
	output[19][3] = ADD(_mm_shuffle_epi32(output[17][3], 0x93), output[18][3]);

	output[20][0] = ADD(_mm_shuffle_epi32(output[18][0], 0x4b), output[19][0]);
	output[20][1] = ADD(_mm_shuffle_epi32(output[18][1], 0x93), output[19][1]);
	output[20][2] = ADD(_mm_shuffle_epi32(output[18][2], 0x4b), output[19][2]);
	output[20][3] = ADD(_mm_shuffle_epi32(output[18][3], 0x93), output[19][3]);

	output[21][0] = ADD(_mm_shuffle_epi32(output[19][0], 0x4b), output[20][0]);
	output[21][1] = ADD(_mm_shuffle_epi32(output[19][1], 0x93), output[20][1]);
	output[21][2] = ADD(_mm_shuffle_epi32(output[19][2], 0x4b), output[20][2]);
	output[21][3] = ADD(_mm_shuffle_epi32(output[19][3], 0x93), output[20][3]);

	output[22][0] = ADD(_mm_shuffle_epi32(output[20][0], 0x4b), output[21][0]);
	output[22][1] = ADD(_mm_shuffle_epi32(output[20][1], 0x93), output[21][1]);
	output[22][2] = ADD(_mm_shuffle_epi32(output[20][2], 0x4b), output[21][2]);
	output[22][3] = ADD(_mm_shuffle_epi32(output[20][3], 0x93), output[21][3]);

	output[23][0] = ADD(_mm_shuffle_epi32(output[21][0], 0x4b), output[22][0]);
	output[23][1] = ADD(_mm_shuffle_epi32(output[21][1], 0x93), output[22][1]);
	output[23][2] = ADD(_mm_shuffle_epi32(output[21][2], 0x4b), output[22][2]);
	output[23][3] = ADD(_mm_shuffle_epi32(output[21][3], 0x93), output[22][3]);

	output[24][0] = ADD(_mm_shuffle_epi32(output[22][0], 0x4b), output[23][0]);
	output[24][1] = ADD(_mm_shuffle_epi32(output[22][1], 0x93), output[23][1]);
	output[24][2] = ADD(_mm_shuffle_epi32(output[22][2], 0x4b), output[23][2]);
	output[24][3] = ADD(_mm_shuffle_epi32(output[22][3], 0x93), output[23][3]);

	output[25][0] = ADD(_mm_shuffle_epi32(output[23][0], 0x4b), output[24][0]);
	output[25][1] = ADD(_mm_shuffle_epi32(output[23][1], 0x93), output[24][1]);
	output[25][2] = ADD(_mm_shuffle_epi32(output[23][2], 0x4b), output[24][2]);
	output[25][3] = ADD(_mm_shuffle_epi32(output[23][3], 0x93), output[24][3]);

	output[26][0] = ADD(_mm_shuffle_epi32(output[24][0], 0x4b), output[25][0]);
	output[26][1] = ADD(_mm_shuffle_epi32(output[24][1], 0x93), output[25][1]);
	output[26][2] = ADD(_mm_shuffle_epi32(output[24][2], 0x4b), output[25][2]);
	output[26][3] = ADD(_mm_shuffle_epi32(output[24][3], 0x93), output[25][3]);

}


static void load_blk(__m128i* dest, const void* src) {
	dest[0] = LOAD((const __m128i*)src);
	dest[1] = LOAD((const __m128i*)src + 1);
}

static void load_sc(__m128i* const_v, word i) {
	load_blk(const_v, SIMD_SC + i);
}



void SIMD_Mix_even_with_permu(__m128i T[4], int Ns, __m128i output[4], __m128i sc[2]) {


	LSH_ALIGNED_(32) __m128i x[2] = { 0, };
	LSH_ALIGNED_(32) __m128i y[2] = { 0, };
	LSH_ALIGNED_(32) __m128i z[2] = { 0, };
	__declspec(align(32)) unsigned int A[4] = { 0,0,0,0 };

	load_sc(sc, Ns * 16);


	z[0] = ADD(T[0], T[2]);
	z[1] = ADD(T[1], T[3]);

	x[0] = XOR(OR(SHIFT_L(z[0], 29), SHIFT_R(z[0], 3)), sc[0]);
	x[1] = XOR(OR(SHIFT_L(z[1], 29), SHIFT_R(z[1], 3)), sc[1]);


	z[0] = ADD(x[0], T[2]);
	y[0] = OR(SHIFT_L(z[0], 1), SHIFT_R(z[0], 31));

	z[1] = ADD(x[1], T[3]);
	y[1] = OR(SHIFT_L(z[1], 1), SHIFT_R(z[1], 31));

	output[0] = _mm_shuffle_epi32((ADD(x[1], y[1])), 0xd2);




	y[1] = _mm_shuffle_epi32(y[1], 0x6c);
	_mm_store_si128((__m128i*)A, y[1]);
	A[0] = ROTL(A[0], 24);
	A[2] = ROTL(A[2], 8);
	A[3] = ROTL(A[3], 16);
	output[1] = LOAD((__m128i*)A);

	output[2] = _mm_shuffle_epi32((ADD(x[0], y[0])), 0xd2);


	y[0] = _mm_shuffle_epi32(y[0], 0x6c);
	_mm_store_si128((__m128i*)A, y[0]);
	A[1] = ROTL(A[1], 24);
	A[2] = ROTL(A[2], 16);
	A[3] = ROTL(A[3], 8);
	output[3] = LOAD((__m128i*)A);


}

void SIMD_Mix_odd_with_permu(__m128i T[4], int Ns, __m128i output[4], __m128i sc[2]) {


	LSH_ALIGNED_(32) __m128i x[2] = { 0, };
	LSH_ALIGNED_(32) __m128i y[2] = { 0, };
	LSH_ALIGNED_(32) __m128i z[2] = { 0, };
	__declspec(align(32)) unsigned int A[4] = { 0,0,0,0 };


	load_sc(sc, Ns * 16 + 8);

	z[0] = ADD(T[0], T[2]);
	z[1] = ADD(T[1], T[3]);

	x[0] = XOR(OR(SHIFT_L(z[0], 5), SHIFT_R(z[0], 27)), sc[0]);
	x[1] = XOR(OR(SHIFT_L(z[1], 5), SHIFT_R(z[1], 27)), sc[1]);

	z[0] = ADD(x[0], T[2]);
	y[0] = OR(SHIFT_L(z[0], 17), SHIFT_R(z[0], 15));

	z[1] = ADD(x[1], T[3]);
	y[1] = OR(SHIFT_L(z[1], 17), SHIFT_R(z[1], 15));


	output[0] = _mm_shuffle_epi32((ADD(x[1], y[1])), 0xd2);

	y[1] = _mm_shuffle_epi32(y[1], 0x6c);
	_mm_store_si128((__m128i*)A, y[1]);
	A[0] = ROTL(A[0], 24);
	A[2] = ROTL(A[2], 8);
	A[3] = ROTL(A[3], 16);
	output[1] = LOAD((__m128i*)A);

	output[2] = _mm_shuffle_epi32((ADD(x[0], y[0])), 0xd2);

	y[0] = _mm_shuffle_epi32(y[0], 0x6c);
	_mm_store_si128((__m128i*)A, y[0]);
	A[1] = ROTL(A[1], 24);
	A[2] = ROTL(A[2], 16);
	A[3] = ROTL(A[3], 8);
	output[3] = LOAD((__m128i*)A);
}

void SIMD_Compression(SIMD_LSH_INFO* info, word pt[32]) {

	int round = 0;

	LSH_ALIGNED_(32) __m128i sc[2] = { 0, };
	LSH_ALIGNED_(32) __m128i temp[4] = { 0, };
	LSH_ALIGNED_(32) __m128i buffer[27][4] = { 0, };

	_mm_store_si128(&temp[0], info->lsh[0]);
	_mm_store_si128(&temp[1], info->lsh[1]);
	_mm_store_si128(&temp[2], info->lsh[2]);
	_mm_store_si128(&temp[3], info->lsh[3]);

	for (int i = 0; i < 8; i++)
		info->buf[i] = LOAD(pt + i);

	SIMD_MsgExp(info->buf, buffer);

	for (round = 0; round < 13; round++) {

		temp[0] = XOR(temp[0], buffer[round << 1][0]);
		temp[1] = XOR(temp[1], buffer[round << 1][1]);
		temp[2] = XOR(temp[2], buffer[round << 1][2]);
		temp[3] = XOR(temp[3], buffer[round << 1][3]);

		SIMD_Mix_even_with_permu(temp, round, temp, sc);

		temp[0] = XOR(temp[0], buffer[(round << 1) + 1][0]);
		temp[1] = XOR(temp[1], buffer[(round << 1) + 1][1]);
		temp[2] = XOR(temp[2], buffer[(round << 1) + 1][2]);
		temp[3] = XOR(temp[3], buffer[(round << 1) + 1][3]);

		SIMD_Mix_odd_with_permu(temp, round, temp, sc);

	}

	info->lsh[0] = _mm_xor_si128(temp[0], buffer[26][0]);
	info->lsh[1] = _mm_xor_si128(temp[1], buffer[26][1]);
	info->lsh[2] = _mm_xor_si128(temp[2], buffer[26][2]);
	info->lsh[3] = _mm_xor_si128(temp[3], buffer[26][3]);


}


void upload_byte2SIMD(IN SIMD_LSH_INFO* info)
{
	word temp[32] = { 0, };
	word value = 0;
	memcpy(temp, info->msg_buf, 128);

	for (int i = 0; i < 8; i++)
		info->buf[i] = LOAD(temp + i);
}

void SIMD_LSH256_init(OUT SIMD_LSH_INFO* info) {


	info->lsh[0] = LOAD((const __m128i*)SIMD_IV);
	info->lsh[1] = LOAD((const __m128i*)SIMD_IV + 1);
	info->lsh[2] = LOAD((const __m128i*)SIMD_IV + 2);
	info->lsh[3] = LOAD((const __m128i*)SIMD_IV + 3);

	info->buf[0] = _mm_setzero_si128();
	info->buf[1] = _mm_setzero_si128();
	info->buf[2] = _mm_setzero_si128();
	info->buf[3] = _mm_setzero_si128();
	info->buf[4] = _mm_setzero_si128();
	info->buf[5] = _mm_setzero_si128();
	info->buf[6] = _mm_setzero_si128();
	info->buf[7] = _mm_setzero_si128();

	info->msglen = 0;
	memset(info->msg_buf, 0, 128);
}

void SIMD_LSH256_Process(IN byte* msg, IN word byte_msglen, OUT SIMD_LSH_INFO* info) {

	unsigned long long cycle2 = 0;
	unsigned long long cycle1 = 0;

	info->msglen += byte_msglen;
	while (byte_msglen >= 128) {
		
		SIMD_Compression(info, (word*)msg);
		msg += 128;
		byte_msglen -= 128;
	}
	memcpy(info->msg_buf, msg, (byte_msglen));
}

void SIMD_LSH256_Final(IN SIMD_LSH_INFO* info, OUT byte* output) {

	int i = 0;
	__declspec(align(32)) unsigned int A[4] = { 0,0,0,0 };
	word final_byte = (info->msglen) % 128;

	((byte*)info->msg_buf)[final_byte++] = 0x80;

	memset(((byte*)info->msg_buf) + final_byte, 0, 128 - final_byte);
;
	SIMD_Compression(info, (word*)info->msg_buf);

	_mm_store_si128((__m128i*)A, _mm_xor_si128(info->lsh[0], info->lsh[2]));
	memcpy(output, A, 16);

	_mm_store_si128((__m128i*)A, _mm_xor_si128(info->lsh[1], info->lsh[3]));
	memcpy(output + 16, A, 16);

}

void SIMD_LSH256(IN const byte* msg, IN word msg_len, OUT byte* output) {
	SIMD_LSH_INFO info;

	unsigned long long cycle1 = 0;
	unsigned long long cycle2 = 0;

	SIMD_LSH256_init(&info);

	SIMD_LSH256_Process(msg, msg_len, &info);

	SIMD_LSH256_Final(&info, output);
}