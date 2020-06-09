#pragma once
int SIMD_KISA_PBKDF2_HMAC_LSH_256(
	IN const byte* password,
	IN const word password_size,
	IN const byte* salt,
	IN const word salt_size,
	IN const word iteration_count,
	IN const word derkey_len,
	OUT		byte* derkey
);

int SIMD_PBKDF2_HMAC_LSH_OP_256(
	IN const byte* password,
	IN const word password_size,
	IN const byte* salt,
	IN const word salt_size,
	IN const word iteration_count,
	IN const word derkey_len,
	OUT		byte* derkey
);

int SIMD_PBKDF2_HMAC_LSH_256_ALL(
	IN const byte* password,
	IN const word password_size,
	IN const byte* salt,
	IN const word salt_size,
	IN const word iteration_count,
	IN const word derkey_len,
	OUT		byte* derkey
);