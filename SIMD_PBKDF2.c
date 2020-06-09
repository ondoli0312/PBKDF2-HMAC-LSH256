#include "Type.h"
#include "LSH256.h"
#include "KISA_LSH_256.h"
#include "SIMD_KISA_LSH_256.h"
#include "PBKDF2.h"
extern word IV[16];
extern  LSH_ALIGNED_(32) word SIMD_IV[16];


int SIMD_KISA_PBKDF2_HMAC_LSH_256(
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
	byte mid_buffer[32] = { 0, };
	byte Buffer[32] = { 0, };
	byte salt_value[1000] = { 0, };
	byte value[1000] = { 0, };
	int Q = 0;
	int R = 0;
	int iter_cnt = 0;
	int updatekeylen = 0;
	int j = 0;
	int i = 1;
	int index = 0;

	Q = derkey_len >> 5;
	R = derkey_len % 32;

	if (password_size > 128)
	{
		lsh256_sse2_digest(32, password, password_size << 3, K0);
		updatekeylen = 32;
	}
	else
	{
		memcpy(K0, password, password_size);
		updatekeylen = password_size;
	}

	memset(K1, IPAD, 128);
	memset(K2, OPAD, 128);

	for (j = 0; j < updatekeylen; j++) {
		K1[j] ^= K0[j];
		K2[j] ^= K0[j];
	}

	memcpy(value, K1, 128);
	memcpy(value+128, salt, salt_size);
	i = 1;
	while ((Q--) > 0)
	{

		value[salt_size + 128] = (i >> 24) & 0xff;
		value[salt_size + 128 + 1] = (i >> 16) & 0xff;
		value[salt_size + 128 + 2] = (i >> 8) & 0xff;
		value[salt_size + 128 + 3] = (i) & 0xff;
		i = i + 1;



		//K1
		lsh256_sse2_digest(32, value, (salt_size + 128 + 4)<<3, FirstOut);
	
		//K2
		memcpy(salt_value, K2, 128);
		memcpy(salt_value + 128, FirstOut, 32);
		lsh256_sse2_digest(32, salt_value, (160) << 3, Finalout);
		memcpy(Buffer, Finalout, 32);
	

		for (iter_cnt = 1; iter_cnt < iteration_count; iter_cnt++)
		{
			memcpy(salt_value, K1, 128);
			memcpy(salt_value+128, Finalout, 32);
			lsh256_sse2_digest(32, salt_value, (160 << 3), FirstOut);
		
			memcpy(salt_value, K2, 128);
			memcpy(salt_value + 128, FirstOut, 32);
			lsh256_sse2_digest(32, salt_value, (160 << 3), Finalout);

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
		salt_value[salt_size + 128] = (i >> 24) & 0xff;
		salt_value[salt_size + 128 + 1] = (i >> 16) & 0xff;
		salt_value[salt_size + 128 + 2] = (i >> 8) & 0xff;
		salt_value[salt_size + 128 + 3] = (i) & 0xff;

		//K1
		lsh256_sse2_digest(32, salt_value, (salt_size + 128 + 4) << 3, FirstOut);

		//K2
		memcpy(salt_value, K2, 128);
		memcpy(salt_value + 128, FirstOut, 32);
		lsh256_sse2_digest(32, salt_value, (160) << 3, Finalout);
		memcpy(Buffer, Finalout, 32);

		for (iter_cnt = 1; iter_cnt < iteration_count; iter_cnt++)
		{
			memcpy(salt_value, K1, 128);
			memcpy(salt_value + 128, Finalout, 32);
			lsh256_sse2_digest(32, salt_value, 160 << 3, FirstOut);

			memcpy(salt_value, K2, 128);
			memcpy(salt_value + 128, FirstOut, 32);
			lsh256_sse2_digest(32, salt_value, 160 << 3, Finalout);

			for (j = 0; j < 32; j++)
				Buffer[j] ^= Finalout[j];

		}
		for (j = 0; j < R; j++, index++)
			derkey[index] = Buffer[j];
	}
}

int SIMD_PBKDF2_HMAC_LSH_OP_256(
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
	byte value[1000] = { 0, };
	int Q = 0;
	int R = 0;
	int iter_cnt = 0;
	int updatekeylen = 0;
	int j = 0;
	int i = 1;
	int index = 0;

	Q = derkey_len >> 5;
	R = derkey_len % 32;

	if (password_size > 128)
	{
		SIMD_LSH256(password, password_size, K0);
		updatekeylen = 32;
	}
	else
	{
		memcpy(K0, password, password_size);
		updatekeylen = password_size;
	}

	memset(K1, IPAD, 128);
	memset(K2, OPAD, 128);

	for (j = 0; j < updatekeylen; j++) {
		K1[j] ^= K0[j];
		K2[j] ^= K0[j];
	}

	memcpy(value, K1, 128);
	memcpy(value + 128, salt, salt_size);

	i = 1;
	while (Q-- > 0)
	{
		value[salt_size + 128] = (i >> 24) & 0xff;
		value[salt_size + 128 + 1] = (i >> 16) & 0xff;
		value[salt_size + 128 + 2] = (i >> 8) & 0xff;
		value[salt_size + 128 + 3] = (i) & 0xff;
		i = i + 1;



		//K1
		SIMD_LSH256(value, (salt_size + 128 + 4), FirstOut);


		//K2
		memcpy(salt_value, K2, 128);
		memcpy(salt_value + 128, FirstOut, 32);
		SIMD_LSH256(salt_value, 160, Finalout);
		memcpy(Buffer, Finalout, 32);
	

		for (iter_cnt = 1; iter_cnt < iteration_count; iter_cnt++)
		{
			memcpy(salt_value, K1, 128);
			memcpy(salt_value + 128, Finalout, 32);
			SIMD_LSH256(salt_value, 160, FirstOut);

			memcpy(salt_value, K2, 128);
			memcpy(salt_value + 128, FirstOut, 32);
			SIMD_LSH256(salt_value, 160, Finalout);

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
		salt_value[salt_size + 128] = (i >> 24) & 0xff;
		salt_value[salt_size + 128 + 1] = (i >> 16) & 0xff;
		salt_value[salt_size + 128 + 2] = (i >> 8) & 0xff;
		salt_value[salt_size + 128 + 3] = (i) & 0xff;

		//K1
		SIMD_LSH256(salt_value, salt_size + 128 + 4, FirstOut);

		//K2
		memcpy(salt_value, K2, 128);
		memcpy(salt_value + 128, FirstOut, 32);
		SIMD_LSH256(salt_value, 160, Finalout);
		memcpy(Buffer, Finalout, 32);

		for (iter_cnt = 1; iter_cnt < iteration_count; iter_cnt++)
		{
			memcpy(salt_value, K1, 128);
			memcpy(salt_value + 128, Finalout, 32);
			SIMD_LSH256(salt_value, 160, FirstOut);

			memcpy(salt_value, K2, 128);
			memcpy(salt_value + 128, FirstOut, 32);
			SIMD_LSH256(salt_value, 160, Finalout);

			for (j = 0; j < 32; j++)
				Buffer[j] ^= Finalout[j];

		}
		for (j = 0; j < R; j++, index++)
			derkey[index] = Buffer[j];
	}
}

void pre_upload_byte2SIMD(byte* K ,__m128i buf[8])
{
	word temp[32] = { 0, };
	word value = 0;
	memcpy(temp, K, 128);

	for (int i = 0; i < 8; i++)
		buf[i] = LOAD(temp + i);
}

void SIMD_precompute_compression(IN byte* K, OUT __m128i state[4])
{

	int round = 0;

	LSH_ALIGNED_(32) __m128i sc[2] = { 0, };
	LSH_ALIGNED_(32) __m128i temp[4] = { 0, };
	LSH_ALIGNED_(32) __m128i buffer[27][4] = { 0, };
	LSH_ALIGNED_(32) __m128i buf[8] = { 0, };
	__declspec(align(32)) unsigned int A[4] = { 0,0,0,0 };


	temp[0] = LOAD((const __m128i*)SIMD_IV);
	temp[1] = LOAD((const __m128i*)SIMD_IV + 1);
	temp[2] = LOAD((const __m128i*)SIMD_IV + 2);
	temp[3] = LOAD((const __m128i*)SIMD_IV + 3);

	pre_upload_byte2SIMD(K, buf);
	SIMD_MsgExp(buf, buffer);


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
	state[0] = _mm_xor_si128(temp[0], buffer[26][0]);
	state[1] = _mm_xor_si128(temp[1], buffer[26][1]);
	state[2] = _mm_xor_si128(temp[2], buffer[26][2]);
	state[3] = _mm_xor_si128(temp[3], buffer[26][3]);
}

void SIMD_ALL_precompute(
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
		SIMD_LSH256(password, password_size, K0);
		updatekeylen = 32;
	}
	else
	{
		memcpy(K0, password, password_size);
		updatekeylen = password_size;
	}

	for (cnt = 0; cnt < updatekeylen; cnt++) {
		K1[cnt] ^= K0[cnt];
		K2[cnt] ^= K0[cnt];
	}

	SIMD_precompute_compression(K1, IPAD_state);
	SIMD_precompute_compression(K2, OPAD_state);

}

void pre_SIMD_LSH256_init(IN __m128i state[4], OUT SIMD_LSH_INFO* info) {


	info->lsh[0] = state[0];
	info->lsh[1] = state[1];
	info->lsh[2] = state[2];
	info->lsh[3] = state[3];

	info->buf[0] = _mm_setzero_si128();
	info->buf[1] = _mm_setzero_si128();
	info->buf[2] = _mm_setzero_si128();
	info->buf[3] = _mm_setzero_si128();
	info->buf[4] = _mm_setzero_si128();
	info->buf[5] = _mm_setzero_si128();
	info->buf[6] = _mm_setzero_si128();
	info->buf[7] = _mm_setzero_si128();

	info->msglen = 128;
	memset(info->msg_buf, 0, 128);
}


void pre_SIMD_Final_process(IN byte* msg, IN word byte_msglen, IN SIMD_LSH_INFO* info, OUT byte* output)
{
	
	int i = 0;
	__declspec(align(32)) unsigned int A[4] = { 0,0,0,0 };
	word final_byte = 32;

	memcpy(info->msg_buf, msg, (byte_msglen));
	((byte*)info->msg_buf)[final_byte++] = 0x80;

	memset(((byte*)info->msg_buf) + final_byte, 0, 128 - final_byte);
	upload_byte2SIMD(info);
	SIMD_Compression(info);

	_mm_store_si128((__m128i*)A, _mm_xor_si128(info->lsh[0], info->lsh[2]));
	memcpy(output, A, 16);

	_mm_store_si128((__m128i*)A, _mm_xor_si128(info->lsh[1], info->lsh[3]));
	memcpy(output + 16, A, 16);
}

int SIMD_PBKDF2_HMAC_LSH_256_ALL(
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

	SIMD_LSH_INFO info;

	int Q = 0;
	int R = 0;
	int iter_cnt = 0;
	int updatekeylen = 0;
	int j = 0;
	int i = 1;
	int index = 0;

	Q = derkey_len >> 5;
	R = derkey_len % 32;

	SIMD_ALL_precompute(password, password_size, IPAD_state, OPAD_state);
	memcpy(salt_value, salt, salt_size);

	i = 1;
	while ((Q--) > 0)
	{
	
		salt_value[salt_size ] = (i >> 24) & 0xff;
		salt_value[salt_size + 1] = (i >> 16) & 0xff;
		salt_value[salt_size + 2] = (i >> 8) & 0xff;
		salt_value[salt_size + 3] = (i) & 0xff;
		i = i + 1;
		
		//K1
		pre_SIMD_LSH256_init(IPAD_state, &info);
		SIMD_LSH256_Process(salt_value, salt_size+ 4, &info);
		SIMD_LSH256_Final(&info, FirstOut);


		//K2
		pre_SIMD_LSH256_init(OPAD_state, &info);
		pre_SIMD_Final_process(FirstOut, 32, &info, Finalout);
		memcpy(Buffer, Finalout, 32);

		for (iter_cnt = 1; iter_cnt < iteration_count; iter_cnt++)
		{
			pre_SIMD_LSH256_init(IPAD_state, &info);
			pre_SIMD_Final_process(Finalout, 32, &info, FirstOut);

			pre_SIMD_LSH256_init(OPAD_state, &info);
			pre_SIMD_Final_process(FirstOut, 32, &info, Finalout);

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
		salt_value[salt_size ] = (i >> 24) & 0xff;
		salt_value[salt_size + 1] = (i >> 16) & 0xff;
		salt_value[salt_size + 2] = (i >> 8) & 0xff;
		salt_value[salt_size + 3] = (i) & 0xff;

		//K1
		pre_SIMD_LSH256_init(IPAD_state, &info);
		SIMD_LSH256_Process(salt_value, salt_size + 4, &info);
		SIMD_LSH256_Final(&info, FirstOut);

		//K2
		pre_SIMD_LSH256_init(OPAD_state, &info);
		pre_SIMD_Final_process(FirstOut, 32, &info, Finalout);
		memcpy(Buffer, Finalout, 32);

		for (iter_cnt = 1; iter_cnt < iteration_count; iter_cnt++)
		{
			pre_SIMD_LSH256_init(IPAD_state, &info);
			pre_SIMD_Final_process(Finalout, 32, &info, FirstOut);

			pre_SIMD_LSH256_init(OPAD_state, &info);
			pre_SIMD_Final_process(FirstOut, 32, &info, Finalout);

			for (j = 0; j < 32; j++)
				Buffer[j] ^= Finalout[j];

		}
		for (j = 0; j < R; j++, index++)
			derkey[index] = Buffer[j];
	}
}