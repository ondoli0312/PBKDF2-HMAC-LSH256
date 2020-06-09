#include <stdio.h>
#include <stdlib.h>
#include "LSH256.h"
#include "Type.h"
#include "SIMD_KISA_LSH_256.h"
#include "KISA_LSH_256.h"
#include "PBKDF2.h"
int main()
{
	byte msg[1024] = { 0x1};
	byte output[64];
	int i = 0;

	unsigned long long cycle = 0;
	unsigned long long cycle1 = 0;
	unsigned long long cycle2 = 0;





	printf("==============================================================================================================\n");

	for (int j = 0; j < 1000; j++)
	{

		cycle2 = cpucycles();
		SIMD_LSH256(msg, sizeof(msg), output);
		cycle1 = cpucycles();
		cycle += (cycle1 - cycle2);
	}


	printf("SIMD = %10lld\n", cycle/1000);

	for (i = 0; i < 32; i++)
		printf("%02x ", output[i]);
	printf("\n");
	cycle2 = 0;
	cycle1 = 0;
	cycle = 0;
	printf("==============================================================================================================\n");

	for (int j = 0; j < 1000; j++)
	{

		cycle2 = cpucycles();	
		lsh256_sse2_digest(32, msg, sizeof(msg) << 3, output);
		cycle1 = cpucycles();
		cycle += (cycle1 - cycle2);
	}

	printf("\n");
	printf("KISA SIMD = %10lld\n", cycle/1000);
	for (i = 0; i < 32; i++)
		printf("%02x ", output[i]);
	printf("\n");
	printf("==============================================================================================================\n");

	cycle2 = 0;
	cycle1 = 0;
#if 0
	printf("==============================================================================================================\n");

	///////////////////////////////////////////////////////////////
	byte password[1024] = { 0x0, };
	byte salt[16] = { 0x0, };
	cycle2 = cpucycles();
	SIMD_KISA_PBKDF2_HMAC_LSH_256(password, sizeof(password), salt, sizeof(salt), 1024, 64, output);
	cycle1 = cpucycles();

	printf("KISA-SIMD-NAIVE = %10lld\n", cycle1 - cycle2);
	for (i = 0; i <64; i++)
		printf("%02x ", output[i]);
	printf("\n");

	printf("\n");

	cycle2 = 0;
	cycle1 = 0;
	cycle2 = cpucycles();
	SIMD_PBKDF2_HMAC_LSH_256_ALL(password, sizeof(password), salt, sizeof(salt), 1024, 64, output);
	cycle1 = cpucycles();

	printf("SIMD - ALL version %10lld\n", cycle1 - cycle2);
	for (i = 0; i < 64; i++)
		printf("%02x ", output[i]);
	printf("\n");
#endif

	byte password[128] = { 0x0, };
	byte salt[128] = { 0x0, };
	cycle = 0;
	for (i = 0; i < 100; i++) {
		cycle2 = cpucycles();
		SIMD_KISA_PBKDF2_HMAC_LSH_256(password, sizeof(password), salt, sizeof(salt), 1024, 32, output);
		cycle1 = cpucycles();
		cycle += cycle1 - cycle2;
	}
	for (i = 0; i < 64; i++)
		printf("%02x ", output[i]);
	printf("\n");
	printf("%10lld\n", cycle);

	cycle = 0;
	for (i = 0; i < 100; i++) {
		cycle2 = cpucycles();
		SIMD_PBKDF2_HMAC_LSH_256_ALL_Use_KISA(password, sizeof(password), salt, sizeof(salt), 1024, 32, output);
		cycle1 = cpucycles();
		cycle += cycle1 - cycle2;
	}
	for (i = 0; i < 64; i++)
		printf("%02x ", output[i]);
	printf("\n");
	printf("%10lld\n", cycle/100);
	
}