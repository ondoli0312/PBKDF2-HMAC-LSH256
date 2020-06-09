#include "Type.h"
#include "LSH256.h"

const word IV[16] = {
	0x46a10f1f, 0xfddce486, 0xb41443a8, 0x198e6b9d, 0x3304388d, 0xb0f5a3c7, 0xb36061c4, 0x7adbd553,
	0x105d5378, 0x2f74de54, 0x5c2f2d95, 0xf2553fbe, 0x8051357a, 0x138668c8, 0x47aa4484, 0xe01afb41
};

const word SC[8 * 26] = {
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


void MsgExp(IN word Msgblock[32], OUT word output[27][16]) {

	output[0][0] = Msgblock[0];
	output[0][1] = Msgblock[1];
	output[0][2] = Msgblock[2];
	output[0][3] = Msgblock[3];
	output[0][4] = Msgblock[4];
	output[0][5] = Msgblock[5];
	output[0][6] = Msgblock[6];
	output[0][7] = Msgblock[7];
	output[0][8] = Msgblock[8];
	output[0][9] = Msgblock[9];
	output[0][10] = Msgblock[10];
	output[0][11] = Msgblock[11];
	output[0][12] = Msgblock[12];
	output[0][13] = Msgblock[13];
	output[0][14] = Msgblock[14];
	output[0][15] = Msgblock[15];

	output[1][0] = Msgblock[16];
	output[1][1] = Msgblock[17];
	output[1][2] = Msgblock[18];
	output[1][3] = Msgblock[19];
	output[1][4] = Msgblock[20];
	output[1][5] = Msgblock[21];
	output[1][6] = Msgblock[22];
	output[1][7] = Msgblock[23];
	output[1][8] = Msgblock[24];
	output[1][9] = Msgblock[25];
	output[1][10] = Msgblock[26];
	output[1][11] = Msgblock[27];
	output[1][12] = Msgblock[28];
	output[1][13] = Msgblock[29];
	output[1][14] = Msgblock[30];
	output[1][15] = Msgblock[31];


	output[2][0] = output[1][0] + output[0][3];
	output[2][1] = output[1][1] + output[0][2];
	output[2][2] = output[1][2] + output[0][0];
	output[2][3] = output[1][3] + output[0][1];
	output[2][4] = output[1][4] + output[0][7];
	output[2][5] = output[1][5] + output[0][4];
	output[2][6] = output[1][6] + output[0][5];
	output[2][7] = output[1][7] + output[0][6];
	output[2][8] = output[1][8] + output[0][11];
	output[2][9] = output[1][9] + output[0][10];
	output[2][10] = output[1][10] + output[0][8];
	output[2][11] = output[1][11] + output[0][9];
	output[2][12] = output[1][12] + output[0][15];
	output[2][13] = output[1][13] + output[0][12];
	output[2][14] = output[1][14] + output[0][13];
	output[2][15] = output[1][15] + output[0][14];


	output[3][0] = output[2][0] + output[1][3];
	output[3][1] = output[2][1] + output[1][2];
	output[3][2] = output[2][2] + output[1][0];
	output[3][3] = output[2][3] + output[1][1];
	output[3][4] = output[2][4] + output[1][7];
	output[3][5] = output[2][5] + output[1][4];
	output[3][6] = output[2][6] + output[1][5];
	output[3][7] = output[2][7] + output[1][6];
	output[3][8] = output[2][8] + output[1][11];
	output[3][9] = output[2][9] + output[1][10];
	output[3][10] = output[2][10] + output[1][8];
	output[3][11] = output[2][11] + output[1][9];
	output[3][12] = output[2][12] + output[1][15];
	output[3][13] = output[2][13] + output[1][12];
	output[3][14] = output[2][14] + output[1][13];
	output[3][15] = output[2][15] + output[1][14];

	output[4][0] = output[3][0] + output[2][3];
	output[4][1] = output[3][1] + output[2][2];
	output[4][2] = output[3][2] + output[2][0];
	output[4][3] = output[3][3] + output[2][1];
	output[4][4] = output[3][4] + output[2][7];
	output[4][5] = output[3][5] + output[2][4];
	output[4][6] = output[3][6] + output[2][5];
	output[4][7] = output[3][7] + output[2][6];
	output[4][8] = output[3][8] + output[2][11];
	output[4][9] = output[3][9] + output[2][10];
	output[4][10] = output[3][10] + output[2][8];
	output[4][11] = output[3][11] + output[2][9];
	output[4][12] = output[3][12] + output[2][15];
	output[4][13] = output[3][13] + output[2][12];
	output[4][14] = output[3][14] + output[2][13];
	output[4][15] = output[3][15] + output[2][14];

	output[5][0] = output[4][0] + output[3][3];
	output[5][1] = output[4][1] + output[3][2];
	output[5][2] = output[4][2] + output[3][0];
	output[5][3] = output[4][3] + output[3][1];
	output[5][4] = output[4][4] + output[3][7];
	output[5][5] = output[4][5] + output[3][4];
	output[5][6] = output[4][6] + output[3][5];
	output[5][7] = output[4][7] + output[3][6];
	output[5][8] = output[4][8] + output[3][11];
	output[5][9] = output[4][9] + output[3][10];
	output[5][10] = output[4][10] + output[3][8];
	output[5][11] = output[4][11] + output[3][9];
	output[5][12] = output[4][12] + output[3][15];
	output[5][13] = output[4][13] + output[3][12];
	output[5][14] = output[4][14] + output[3][13];
	output[5][15] = output[4][15] + output[3][14];

	output[6][0] = output[5][0] + output[4][3];
	output[6][1] = output[5][1] + output[4][2];
	output[6][2] = output[5][2] + output[4][0];
	output[6][3] = output[5][3] + output[4][1];
	output[6][4] = output[5][4] + output[4][7];
	output[6][5] = output[5][5] + output[4][4];
	output[6][6] = output[5][6] + output[4][5];
	output[6][7] = output[5][7] + output[4][6];
	output[6][8] = output[5][8] + output[4][11];
	output[6][9] = output[5][9] + output[4][10];
	output[6][10] = output[5][10] + output[4][8];
	output[6][11] = output[5][11] + output[4][9];
	output[6][12] = output[5][12] + output[4][15];
	output[6][13] = output[5][13] + output[4][12];
	output[6][14] = output[5][14] + output[4][13];
	output[6][15] = output[5][15] + output[4][14];

	output[7][0] = output[6][0] + output[5][3];
	output[7][1] = output[6][1] + output[5][2];
	output[7][2] = output[6][2] + output[5][0];
	output[7][3] = output[6][3] + output[5][1];
	output[7][4] = output[6][4] + output[5][7];
	output[7][5] = output[6][5] + output[5][4];
	output[7][6] = output[6][6] + output[5][5];
	output[7][7] = output[6][7] + output[5][6];
	output[7][8] = output[6][8] + output[5][11];
	output[7][9] = output[6][9] + output[5][10];
	output[7][10] = output[6][10] + output[5][8];
	output[7][11] = output[6][11] + output[5][9];
	output[7][12] = output[6][12] + output[5][15];
	output[7][13] = output[6][13] + output[5][12];
	output[7][14] = output[6][14] + output[5][13];
	output[7][15] = output[6][15] + output[5][14];

	output[8][0] = output[7][0] + output[6][3];
	output[8][1] = output[7][1] + output[6][2];
	output[8][2] = output[7][2] + output[6][0];
	output[8][3] = output[7][3] + output[6][1];
	output[8][4] = output[7][4] + output[6][7];
	output[8][5] = output[7][5] + output[6][4];
	output[8][6] = output[7][6] + output[6][5];
	output[8][7] = output[7][7] + output[6][6];
	output[8][8] = output[7][8] + output[6][11];
	output[8][9] = output[7][9] + output[6][10];
	output[8][10] = output[7][10] + output[6][8];
	output[8][11] = output[7][11] + output[6][9];
	output[8][12] = output[7][12] + output[6][15];
	output[8][13] = output[7][13] + output[6][12];
	output[8][14] = output[7][14] + output[6][13];
	output[8][15] = output[7][15] + output[6][14];

	output[9][0] = output[8][0] + output[7][3];
	output[9][1] = output[8][1] + output[7][2];
	output[9][2] = output[8][2] + output[7][0];
	output[9][3] = output[8][3] + output[7][1];
	output[9][4] = output[8][4] + output[7][7];
	output[9][5] = output[8][5] + output[7][4];
	output[9][6] = output[8][6] + output[7][5];
	output[9][7] = output[8][7] + output[7][6];
	output[9][8] = output[8][8] + output[7][11];
	output[9][9] = output[8][9] + output[7][10];
	output[9][10] = output[8][10] + output[7][8];
	output[9][11] = output[8][11] + output[7][9];
	output[9][12] = output[8][12] + output[7][15];
	output[9][13] = output[8][13] + output[7][12];
	output[9][14] = output[8][14] + output[7][13];
	output[9][15] = output[8][15] + output[7][14];

	output[10][0] = output[9][0] + output[8][3];
	output[10][1] = output[9][1] + output[8][2];
	output[10][2] = output[9][2] + output[8][0];
	output[10][3] = output[9][3] + output[8][1];
	output[10][4] = output[9][4] + output[8][7];
	output[10][5] = output[9][5] + output[8][4];
	output[10][6] = output[9][6] + output[8][5];
	output[10][7] = output[9][7] + output[8][6];
	output[10][8] = output[9][8] + output[8][11];
	output[10][9] = output[9][9] + output[8][10];
	output[10][10] = output[9][10] + output[8][8];
	output[10][11] = output[9][11] + output[8][9];
	output[10][12] = output[9][12] + output[8][15];
	output[10][13] = output[9][13] + output[8][12];
	output[10][14] = output[9][14] + output[8][13];
	output[10][15] = output[9][15] + output[8][14];

	output[11][0] = output[10][0] + output[9][3];
	output[11][1] = output[10][1] + output[9][2];
	output[11][2] = output[10][2] + output[9][0];
	output[11][3] = output[10][3] + output[9][1];
	output[11][4] = output[10][4] + output[9][7];
	output[11][5] = output[10][5] + output[9][4];
	output[11][6] = output[10][6] + output[9][5];
	output[11][7] = output[10][7] + output[9][6];
	output[11][8] = output[10][8] + output[9][11];
	output[11][9] = output[10][9] + output[9][10];
	output[11][10] = output[10][10] + output[9][8];
	output[11][11] = output[10][11] + output[9][9];
	output[11][12] = output[10][12] + output[9][15];
	output[11][13] = output[10][13] + output[9][12];
	output[11][14] = output[10][14] + output[9][13];
	output[11][15] = output[10][15] + output[9][14];

	output[12][0] = output[11][0] + output[10][3];
	output[12][1] = output[11][1] + output[10][2];
	output[12][2] = output[11][2] + output[10][0];
	output[12][3] = output[11][3] + output[10][1];
	output[12][4] = output[11][4] + output[10][7];
	output[12][5] = output[11][5] + output[10][4];
	output[12][6] = output[11][6] + output[10][5];
	output[12][7] = output[11][7] + output[10][6];
	output[12][8] = output[11][8] + output[10][11];
	output[12][9] = output[11][9] + output[10][10];
	output[12][10] = output[11][10] + output[10][8];
	output[12][11] = output[11][11] + output[10][9];
	output[12][12] = output[11][12] + output[10][15];
	output[12][13] = output[11][13] + output[10][12];
	output[12][14] = output[11][14] + output[10][13];
	output[12][15] = output[11][15] + output[10][14];

	output[13][0] = output[12][0] + output[11][3];
	output[13][1] = output[12][1] + output[11][2];
	output[13][2] = output[12][2] + output[11][0];
	output[13][3] = output[12][3] + output[11][1];
	output[13][4] = output[12][4] + output[11][7];
	output[13][5] = output[12][5] + output[11][4];
	output[13][6] = output[12][6] + output[11][5];
	output[13][7] = output[12][7] + output[11][6];
	output[13][8] = output[12][8] + output[11][11];
	output[13][9] = output[12][9] + output[11][10];
	output[13][10] = output[12][10] + output[11][8];
	output[13][11] = output[12][11] + output[11][9];
	output[13][12] = output[12][12] + output[11][15];
	output[13][13] = output[12][13] + output[11][12];
	output[13][14] = output[12][14] + output[11][13];
	output[13][15] = output[12][15] + output[11][14];

	output[14][0] = output[13][0] + output[12][3];
	output[14][1] = output[13][1] + output[12][2];
	output[14][2] = output[13][2] + output[12][0];
	output[14][3] = output[13][3] + output[12][1];
	output[14][4] = output[13][4] + output[12][7];
	output[14][5] = output[13][5] + output[12][4];
	output[14][6] = output[13][6] + output[12][5];
	output[14][7] = output[13][7] + output[12][6];
	output[14][8] = output[13][8] + output[12][11];
	output[14][9] = output[13][9] + output[12][10];
	output[14][10] = output[13][10] + output[12][8];
	output[14][11] = output[13][11] + output[12][9];
	output[14][12] = output[13][12] + output[12][15];
	output[14][13] = output[13][13] + output[12][12];
	output[14][14] = output[13][14] + output[12][13];
	output[14][15] = output[13][15] + output[12][14];

	output[15][0] = output[14][0] + output[13][3];
	output[15][1] = output[14][1] + output[13][2];
	output[15][2] = output[14][2] + output[13][0];
	output[15][3] = output[14][3] + output[13][1];
	output[15][4] = output[14][4] + output[13][7];
	output[15][5] = output[14][5] + output[13][4];
	output[15][6] = output[14][6] + output[13][5];
	output[15][7] = output[14][7] + output[13][6];
	output[15][8] = output[14][8] + output[13][11];
	output[15][9] = output[14][9] + output[13][10];
	output[15][10] = output[14][10] + output[13][8];
	output[15][11] = output[14][11] + output[13][9];
	output[15][12] = output[14][12] + output[13][15];
	output[15][13] = output[14][13] + output[13][12];
	output[15][14] = output[14][14] + output[13][13];
	output[15][15] = output[14][15] + output[13][14];

	output[16][0] = output[15][0] + output[14][3];
	output[16][1] = output[15][1] + output[14][2];
	output[16][2] = output[15][2] + output[14][0];
	output[16][3] = output[15][3] + output[14][1];
	output[16][4] = output[15][4] + output[14][7];
	output[16][5] = output[15][5] + output[14][4];
	output[16][6] = output[15][6] + output[14][5];
	output[16][7] = output[15][7] + output[14][6];
	output[16][8] = output[15][8] + output[14][11];
	output[16][9] = output[15][9] + output[14][10];
	output[16][10] = output[15][10] + output[14][8];
	output[16][11] = output[15][11] + output[14][9];
	output[16][12] = output[15][12] + output[14][15];
	output[16][13] = output[15][13] + output[14][12];
	output[16][14] = output[15][14] + output[14][13];
	output[16][15] = output[15][15] + output[14][14];

	output[17][0] = output[16][0] + output[15][3];
	output[17][1] = output[16][1] + output[15][2];
	output[17][2] = output[16][2] + output[15][0];
	output[17][3] = output[16][3] + output[15][1];
	output[17][4] = output[16][4] + output[15][7];
	output[17][5] = output[16][5] + output[15][4];
	output[17][6] = output[16][6] + output[15][5];
	output[17][7] = output[16][7] + output[15][6];
	output[17][8] = output[16][8] + output[15][11];
	output[17][9] = output[16][9] + output[15][10];
	output[17][10] = output[16][10] + output[15][8];
	output[17][11] = output[16][11] + output[15][9];
	output[17][12] = output[16][12] + output[15][15];
	output[17][13] = output[16][13] + output[15][12];
	output[17][14] = output[16][14] + output[15][13];
	output[17][15] = output[16][15] + output[15][14];

	output[18][0] = output[17][0] + output[16][3];
	output[18][1] = output[17][1] + output[16][2];
	output[18][2] = output[17][2] + output[16][0];
	output[18][3] = output[17][3] + output[16][1];
	output[18][4] = output[17][4] + output[16][7];
	output[18][5] = output[17][5] + output[16][4];
	output[18][6] = output[17][6] + output[16][5];
	output[18][7] = output[17][7] + output[16][6];
	output[18][8] = output[17][8] + output[16][11];
	output[18][9] = output[17][9] + output[16][10];
	output[18][10] = output[17][10] + output[16][8];
	output[18][11] = output[17][11] + output[16][9];
	output[18][12] = output[17][12] + output[16][15];
	output[18][13] = output[17][13] + output[16][12];
	output[18][14] = output[17][14] + output[16][13];
	output[18][15] = output[17][15] + output[16][14];

	output[19][0] = output[18][0] + output[17][3];
	output[19][1] = output[18][1] + output[17][2];
	output[19][2] = output[18][2] + output[17][0];
	output[19][3] = output[18][3] + output[17][1];
	output[19][4] = output[18][4] + output[17][7];
	output[19][5] = output[18][5] + output[17][4];
	output[19][6] = output[18][6] + output[17][5];
	output[19][7] = output[18][7] + output[17][6];
	output[19][8] = output[18][8] + output[17][11];
	output[19][9] = output[18][9] + output[17][10];
	output[19][10] = output[18][10] + output[17][8];
	output[19][11] = output[18][11] + output[17][9];
	output[19][12] = output[18][12] + output[17][15];
	output[19][13] = output[18][13] + output[17][12];
	output[19][14] = output[18][14] + output[17][13];
	output[19][15] = output[18][15] + output[17][14];

	output[20][0] = output[19][0] + output[18][3];
	output[20][1] = output[19][1] + output[18][2];
	output[20][2] = output[19][2] + output[18][0];
	output[20][3] = output[19][3] + output[18][1];
	output[20][4] = output[19][4] + output[18][7];
	output[20][5] = output[19][5] + output[18][4];
	output[20][6] = output[19][6] + output[18][5];
	output[20][7] = output[19][7] + output[18][6];
	output[20][8] = output[19][8] + output[18][11];
	output[20][9] = output[19][9] + output[18][10];
	output[20][10] = output[19][10] + output[18][8];
	output[20][11] = output[19][11] + output[18][9];
	output[20][12] = output[19][12] + output[18][15];
	output[20][13] = output[19][13] + output[18][12];
	output[20][14] = output[19][14] + output[18][13];
	output[20][15] = output[19][15] + output[18][14];

	output[21][0] = output[20][0] + output[19][3];
	output[21][1] = output[20][1] + output[19][2];
	output[21][2] = output[20][2] + output[19][0];
	output[21][3] = output[20][3] + output[19][1];
	output[21][4] = output[20][4] + output[19][7];
	output[21][5] = output[20][5] + output[19][4];
	output[21][6] = output[20][6] + output[19][5];
	output[21][7] = output[20][7] + output[19][6];
	output[21][8] = output[20][8] + output[19][11];
	output[21][9] = output[20][9] + output[19][10];
	output[21][10] = output[20][10] + output[19][8];
	output[21][11] = output[20][11] + output[19][9];
	output[21][12] = output[20][12] + output[19][15];
	output[21][13] = output[20][13] + output[19][12];
	output[21][14] = output[20][14] + output[19][13];
	output[21][15] = output[20][15] + output[19][14];

	output[22][0] = output[21][0] + output[20][3];
	output[22][1] = output[21][1] + output[20][2];
	output[22][2] = output[21][2] + output[20][0];
	output[22][3] = output[21][3] + output[20][1];
	output[22][4] = output[21][4] + output[20][7];
	output[22][5] = output[21][5] + output[20][4];
	output[22][6] = output[21][6] + output[20][5];
	output[22][7] = output[21][7] + output[20][6];
	output[22][8] = output[21][8] + output[20][11];
	output[22][9] = output[21][9] + output[20][10];
	output[22][10] = output[21][10] + output[20][8];
	output[22][11] = output[21][11] + output[20][9];
	output[22][12] = output[21][12] + output[20][15];
	output[22][13] = output[21][13] + output[20][12];
	output[22][14] = output[21][14] + output[20][13];
	output[22][15] = output[21][15] + output[20][14];

	output[23][0] = output[22][0] + output[21][3];
	output[23][1] = output[22][1] + output[21][2];
	output[23][2] = output[22][2] + output[21][0];
	output[23][3] = output[22][3] + output[21][1];
	output[23][4] = output[22][4] + output[21][7];
	output[23][5] = output[22][5] + output[21][4];
	output[23][6] = output[22][6] + output[21][5];
	output[23][7] = output[22][7] + output[21][6];
	output[23][8] = output[22][8] + output[21][11];
	output[23][9] = output[22][9] + output[21][10];
	output[23][10] = output[22][10] + output[21][8];
	output[23][11] = output[22][11] + output[21][9];
	output[23][12] = output[22][12] + output[21][15];
	output[23][13] = output[22][13] + output[21][12];
	output[23][14] = output[22][14] + output[21][13];
	output[23][15] = output[22][15] + output[21][14];

	output[24][0] = output[23][0] + output[22][3];
	output[24][1] = output[23][1] + output[22][2];
	output[24][2] = output[23][2] + output[22][0];
	output[24][3] = output[23][3] + output[22][1];
	output[24][4] = output[23][4] + output[22][7];
	output[24][5] = output[23][5] + output[22][4];
	output[24][6] = output[23][6] + output[22][5];
	output[24][7] = output[23][7] + output[22][6];
	output[24][8] = output[23][8] + output[22][11];
	output[24][9] = output[23][9] + output[22][10];
	output[24][10] = output[23][10] + output[22][8];
	output[24][11] = output[23][11] + output[22][9];
	output[24][12] = output[23][12] + output[22][15];
	output[24][13] = output[23][13] + output[22][12];
	output[24][14] = output[23][14] + output[22][13];
	output[24][15] = output[23][15] + output[22][14];

	output[25][0] = output[24][0] + output[23][3];
	output[25][1] = output[24][1] + output[23][2];
	output[25][2] = output[24][2] + output[23][0];
	output[25][3] = output[24][3] + output[23][1];
	output[25][4] = output[24][4] + output[23][7];
	output[25][5] = output[24][5] + output[23][4];
	output[25][6] = output[24][6] + output[23][5];
	output[25][7] = output[24][7] + output[23][6];
	output[25][8] = output[24][8] + output[23][11];
	output[25][9] = output[24][9] + output[23][10];
	output[25][10] = output[24][10] + output[23][8];
	output[25][11] = output[24][11] + output[23][9];
	output[25][12] = output[24][12] + output[23][15];
	output[25][13] = output[24][13] + output[23][12];
	output[25][14] = output[24][14] + output[23][13];
	output[25][15] = output[24][15] + output[23][14];

	output[26][0] = output[25][0] + output[24][3];
	output[26][1] = output[25][1] + output[24][2];
	output[26][2] = output[25][2] + output[24][0];
	output[26][3] = output[25][3] + output[24][1];
	output[26][4] = output[25][4] + output[24][7];
	output[26][5] = output[25][5] + output[24][4];
	output[26][6] = output[25][6] + output[24][5];
	output[26][7] = output[25][7] + output[24][6];
	output[26][8] = output[25][8] + output[24][11];
	output[26][9] = output[25][9] + output[24][10];
	output[26][10] = output[25][10] + output[24][8];
	output[26][11] = output[25][11] + output[24][9];
	output[26][12] = output[25][12] + output[24][15];
	output[26][13] = output[25][13] + output[24][12];
	output[26][14] = output[25][14] + output[24][13];
	output[26][15] = output[25][15] + output[24][14];

}


void Mix_even_with_permu(word T[16], int Ns, word output[16]) {

	word X[8];
	word Y[8];

	X[0] = (ROTL((T[0] + T[8]), 29)) ^ SC[(Ns << 3)];
	X[1] = (ROTL((T[1] + T[9]), 29)) ^ SC[(Ns << 3) + 1];
	X[2] = (ROTL((T[2] + T[10]), 29)) ^ SC[(Ns << 3) + 2];
	X[3] = (ROTL(T[3] + T[11], 29)) ^ SC[(Ns << 3) + 3];


	X[4] = (ROTL(T[4] + T[12], 29)) ^ SC[(Ns << 3) + 4];
	X[5] = (ROTL(T[5] + T[13], 29)) ^ SC[(Ns << 3) + 5];
	X[6] = (ROTL(T[6] + T[14], 29)) ^ SC[(Ns << 3) + 6];
	X[7] = (ROTL(T[7] + T[15], 29)) ^ SC[(Ns << 3) + 7];



	Y[0] = ROTL(X[0] + T[8], 1);
	Y[1] = ROTL(X[1] + T[9], 1);
	Y[2] = ROTL(X[2] + T[10], 1);
	Y[3] = ROTL(X[3] + T[11], 1);

	Y[4] = ROTL(X[4] + T[12], 1);
	Y[5] = ROTL(X[5] + T[13], 1);
	Y[6] = ROTL(X[6] + T[14], 1);
	Y[7] = ROTL(X[7] + T[15], 1);

	output[0] = X[6] + Y[6];
	output[1] = X[4] + Y[4];
	output[2] = X[5] + Y[5];
	output[3] = X[7] + Y[7];




	output[4] = ROTL(Y[4], 24);
	output[5] = Y[7];
	output[6] = ROTL(Y[6], 8);
	output[7] = ROTL(Y[5], 16);



	output[8] = X[2] + Y[2];
	output[9] = X[0] + Y[0];
	output[10] = X[1] + Y[1];
	output[11] = X[3] + Y[3];



	output[12] = Y[0];
	output[13] = ROTL(Y[3], 24);
	output[14] = ROTL(Y[2], 16);
	output[15] = ROTL(Y[1], 8);

}

void Mix_odd_with_permu(word T[16], int Ns, word output[16]) {

	word X[8];
	word Y[8];

	X[0] = (ROTL((T[0] + T[8]), 5)) ^ SC[Ns * 8];
	X[1] = (ROTL((T[1] + T[9]), 5)) ^ SC[Ns * 8 + 1];
	X[2] = (ROTL((T[2] + T[10]), 5)) ^ SC[Ns * 8 + 2];
	X[3] = (ROTL(T[3] + T[11], 5)) ^ SC[Ns * 8 + 3];

	X[4] = (ROTL(T[4] + T[12], 5)) ^ SC[Ns * 8 + 4];
	X[5] = (ROTL(T[5] + T[13], 5)) ^ SC[Ns * 8 + 5];
	X[6] = (ROTL(T[6] + T[14], 5)) ^ SC[Ns * 8 + 6];
	X[7] = (ROTL(T[7] + T[15], 5)) ^ SC[Ns * 8 + 7];

	Y[0] = ROTL(X[0] + T[8], 17);
	Y[1] = ROTL(X[1] + T[9], 17);
	Y[2] = ROTL(X[2] + T[10], 17);
	Y[3] = ROTL(X[3] + T[11], 17);
	Y[4] = ROTL(X[4] + T[12], 17);
	Y[5] = ROTL(X[5] + T[13], 17);
	Y[6] = ROTL(X[6] + T[14], 17);
	Y[7] = ROTL(X[7] + T[15], 17);

	output[0] = X[6] + Y[6];
	output[1] = X[4] + Y[4];
	output[2] = X[5] + Y[5];
	output[3] = X[7] + Y[7];


	//printf("%08x, %08x, %08x, %08x\n", output[0], output[1], output[2], output[3]);

	output[4] = ROTL(Y[4], 24);
	output[5] = Y[7];
	output[6] = ROTL(Y[6], 8);
	output[7] = ROTL(Y[5], 16);

	output[8] = X[2] + Y[2];
	output[9] = X[0] + Y[0];
	output[10] = X[1] + Y[1];
	output[11] = X[3] + Y[3];

	output[12] = Y[0];
	output[13] = ROTL(Y[3], 24);
	output[14] = ROTL(Y[2], 16);
	output[15] = ROTL(Y[1], 8);


}

void Compression(LSH256_INFO* info) {

	word Temp[16];
	word buffer[27][16];
	int round = 0;
	int z = 0;

	memcpy(Temp, (word*)info->lsh, sizeof(word) * 16);
	MsgExp((word*)info->buf, buffer);

	for (round = 0; round < 13; round++) {
		Temp[0] ^= buffer[round << 1][0];
		Temp[1] ^= buffer[round << 1][1];
		Temp[2] ^= buffer[round << 1][2];
		Temp[3] ^= buffer[round << 1][3];


		Temp[4] ^= buffer[round << 1][4];
		Temp[5] ^= buffer[round << 1][5];
		Temp[6] ^= buffer[round << 1][6];
		Temp[7] ^= buffer[round << 1][7];

		Temp[8] ^= buffer[round << 1][8];
		Temp[9] ^= buffer[round << 1][9];
		Temp[10] ^= buffer[round << 1][10];
		Temp[11] ^= buffer[round << 1][11];

		Temp[12] ^= buffer[round << 1][12];
		Temp[13] ^= buffer[round << 1][13];
		Temp[14] ^= buffer[round << 1][14];
		Temp[15] ^= buffer[round << 1][15];

		Mix_even_with_permu(Temp, round << 1, Temp);




		Temp[0] ^= buffer[(round << 1) + 1][0];
		Temp[1] ^= buffer[(round << 1) + 1][1];
		Temp[2] ^= buffer[(round << 1) + 1][2];
		Temp[3] ^= buffer[(round << 1) + 1][3];


		Temp[4] ^= buffer[(round << 1) + 1][4];
		Temp[5] ^= buffer[(round << 1) + 1][5];
		Temp[6] ^= buffer[(round << 1) + 1][6];
		Temp[7] ^= buffer[(round << 1) + 1][7];

		Temp[8] ^= buffer[(round << 1) + 1][8];
		Temp[9] ^= buffer[(round << 1) + 1][9];
		Temp[10] ^= buffer[(round << 1) + 1][10];
		Temp[11] ^= buffer[(round << 1) + 1][11];

		Temp[12] ^= buffer[(round << 1) + 1][12];
		Temp[13] ^= buffer[(round << 1) + 1][13];
		Temp[14] ^= buffer[(round << 1) + 1][14];
		Temp[15] ^= buffer[(round << 1) + 1][15];




		Mix_odd_with_permu(Temp, (round << 1) + 1, Temp);




	}

	info->lsh[0] = Temp[0] ^ buffer[26][0];
	info->lsh[1] = Temp[1] ^ buffer[26][1];
	info->lsh[2] = Temp[2] ^ buffer[26][2];
	info->lsh[3] = Temp[3] ^ buffer[26][3];



	info->lsh[4] = Temp[4] ^ buffer[26][4];
	info->lsh[5] = Temp[5] ^ buffer[26][5];
	info->lsh[6] = Temp[6] ^ buffer[26][6];
	info->lsh[7] = Temp[7] ^ buffer[26][7];


	info->lsh[8] = Temp[8] ^ buffer[26][8];
	info->lsh[9] = Temp[9] ^ buffer[26][9];
	info->lsh[10] = Temp[10] ^ buffer[26][10];
	info->lsh[11] = Temp[11] ^ buffer[26][11];



	info->lsh[12] = Temp[12] ^ buffer[26][12];
	info->lsh[13] = Temp[13] ^ buffer[26][13];
	info->lsh[14] = Temp[14] ^ buffer[26][14];
	info->lsh[15] = Temp[15] ^ buffer[26][15];



}


void LSH256_init(OUT LSH256_INFO* info) {

	info->lsh[0] = IV[0];
	info->lsh[1] = IV[1];
	info->lsh[2] = IV[2];
	info->lsh[3] = IV[3];
	info->lsh[4] = IV[4];
	info->lsh[5] = IV[5];
	info->lsh[6] = IV[6];
	info->lsh[7] = IV[7];
	info->lsh[8] = IV[8];
	info->lsh[9] = IV[9];
	info->lsh[10] = IV[10];
	info->lsh[11] = IV[11];
	info->lsh[12] = IV[12];
	info->lsh[13] = IV[13];
	info->lsh[14] = IV[14];
	info->lsh[15] = IV[15];
	info->byte_msglen = 0;

	memset((word*)info->buf, 0, sizeof(word) * 32);

}

void LSH256_Process(IN byte* msg, IN word byte_msglen, OUT LSH256_INFO* info) {

	info->byte_msglen += byte_msglen;
	while (byte_msglen >= 128) {
		memcpy(((byte*)info->buf), (msg), 128);
		Compression(info);
		msg += 128;
		byte_msglen -= 128;
	}
	memcpy((byte*)info->buf, msg, (byte_msglen));
}

void LSH256_Final(IN LSH256_INFO* info, OUT byte* output) {

	word temp = 0;
	int i = 0;


	word final_byte = (info->byte_msglen) % 128;

	((byte*)info->buf)[final_byte++] = 0x80;

	memset(((byte*)info->buf) + final_byte, 0, 128 - final_byte);

	Compression(info);

	temp = ENDIAN_CHANGE((info->lsh[0]) ^ (info->lsh[8]));



	output[0] = ((temp) >> 24) & 0xff;
	output[1] = ((temp) >> 16) & 0xff;
	output[2] = ((temp) >> 8) & 0xff;
	output[3] = (temp) & 0xff;

	temp = ENDIAN_CHANGE((info->lsh[1]) ^ (info->lsh[9]));
	output[4] = ((temp) >> 24) & 0xff;
	output[5] = ((temp) >> 16) & 0xff;
	output[6] = ((temp) >> 8) & 0xff;
	output[7] = (temp) & 0xff;

	temp = ENDIAN_CHANGE((info->lsh[2]) ^ (info->lsh[10]));
	output[8] = ((temp) >> 24) & 0xff;
	output[9] = ((temp) >> 16) & 0xff;
	output[10] = ((temp) >> 8) & 0xff;
	output[11] = (temp) & 0xff;

	temp = ENDIAN_CHANGE((info->lsh[3]) ^ (info->lsh[11]));
	output[12] = ((temp) >> 24) & 0xff;
	output[13] = ((temp) >> 16) & 0xff;
	output[14] = ((temp) >> 8) & 0xff;
	output[15] = (temp) & 0xff;


	temp = ENDIAN_CHANGE((info->lsh[4]) ^ (info->lsh[12]));
	output[16] = ((temp) >> 24) & 0xff;
	output[17] = ((temp) >> 16) & 0xff;
	output[18] = ((temp) >> 8) & 0xff;
	output[19] = (temp) & 0xff;

	temp = ENDIAN_CHANGE((info->lsh[5]) ^ (info->lsh[13]));
	output[20] = ((temp) >> 24) & 0xff;
	output[21] = ((temp) >> 16) & 0xff;
	output[22] = ((temp) >> 8) & 0xff;
	output[23] = (temp) & 0xff;

	temp = ENDIAN_CHANGE((info->lsh[6]) ^ (info->lsh[14]));
	output[24] = ((temp) >> 24) & 0xff;
	output[25] = ((temp) >> 16) & 0xff;
	output[26] = ((temp) >> 8) & 0xff;
	output[27] = (temp) & 0xff;

	temp = ENDIAN_CHANGE((info->lsh[7]) ^ (info->lsh[15]));
	output[28] = ((temp) >> 24) & 0xff;
	output[29] = ((temp) >> 16) & 0xff;
	output[30] = ((temp) >> 8) & 0xff;
	output[31] = (temp) & 0xff;

}

void LSH256(IN const byte* msg, IN word msg_len, OUT byte* output) {
	LSH256_INFO info;
	LSH256_init(&info);
	LSH256_Process(msg, msg_len, &info);
	LSH256_Final(&info, output);
}