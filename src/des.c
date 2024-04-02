#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

static const uint8_t des_pc_table_1[56] = {
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4
};

static const uint8_t des_pc_table_2[48] = {
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

static const uint8_t des_ip_table[64] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

static const uint8_t des_e_table[48] = {
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1
};

static const uint8_t des_s1_table[4][16] = {
	{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
	{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
	{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
	{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
};

static const uint8_t des_s2_table[4][16] = {
	{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
	{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
	{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
	{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
};

static const uint8_t des_s3_table[4][16] = {
	{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
	{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
	{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
	{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
};

static const uint8_t des_s4_table[4][16] = {
	{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
	{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
	{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
	{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
};

static const uint8_t des_s5_table[4][16] = {
	{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
	{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
	{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
	{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
};

static const uint8_t des_s6_table[4][16] = {
	{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
	{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
	{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
	{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
};

static const uint8_t des_s7_table[4][16] = {
	{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
	{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
	{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
	{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
};

static const uint8_t des_s8_table[4][16] = {
	{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
	{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
	{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
	{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
};

static const uint8_t (*des_meta_s_table[8])[16] = {
	des_s1_table, des_s2_table, des_s3_table, des_s4_table,
	des_s5_table, des_s6_table, des_s7_table, des_s8_table
};

static const uint8_t des_p_table[32] = {
	16, 7, 20, 21, 29, 12, 28, 17,
	1, 15, 23, 26, 5, 18, 31, 10,
	2, 8, 24, 14, 32, 27, 3, 9,
	19, 13, 30, 6, 22, 11, 4, 25
};

static const uint8_t des_ip_invert_table[64] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25
};

static const uint8_t des_ls_order[16] = {
	1, 1, 2, 2, 2, 2, 2, 2, 
	1, 2, 2, 2, 2, 2, 2, 1,
};

static uint64_t e(uint32_t val)
{
	uint64_t tmp = 0;
	for (int i = 0; i < 48; i++) {
		int bit = (val >> (32 - des_e_table[i])) & 1;
		if (bit)
			tmp |= (1ULL << (47 - i));
	}
	return tmp;
}

static int32_t p(uint32_t val)
{
	uint64_t tmp = 0;
	for (int i = 0; i < 32; i++) {
		int bit = (val >> (32 - des_p_table[i])) & 1;
		if (bit)
			tmp |= (1ULL << (31 - i));
	}
	return tmp;
}

static uint32_t f(uint32_t data, uint64_t key)
{
	// 48 bits
	uint64_t tmp = key ^ e(data);

	uint32_t result = 0;
	for (int i = 0; i < 8; i++) {
		uint8_t bits = (tmp >> (i * 6)) & 0x3f;
		uint8_t row_idx = ((bits & 0x20) >> 4) | (bits & 1);
		uint8_t col_idx = (bits & 0x1e) >> 1;
		uint8_t s_result = des_meta_s_table[7 - i][row_idx][col_idx];
		result |= (s_result << (i * 4));
	}

	return p(result);
}

uint64_t des(uint64_t msg, uint64_t key, bool decrypt)
{
	uint64_t k_plus = 0;

	for (int i = 0; i < 56; i++) {
		int bit = (key >> (64 - des_pc_table_1[i])) & 1;
		if (bit)
			k_plus |= (1ULL << (55 - i));
	}

	uint32_t c0 = (k_plus >> 28) & 0x0fffffff;
	uint32_t d0 = k_plus & 0x0fffffff;

	uint32_t c0_16[17];
	uint32_t d0_16[17];
	c0_16[0] = c0;
	d0_16[0] = d0;

	for (int i = 1; i < 17; i++) {
		int iter_num_shifts = des_ls_order[i - 1];
		for (int j = 0; j < iter_num_shifts; j++) {
			uint32_t cN_minus_one = (iter_num_shifts == 2) && (j == 1) ? c0_16[i] : c0_16[i - 1];
			int cN_last_bit = (cN_minus_one >> 27) & 1;
			c0_16[i] = ((cN_minus_one << 1) & 0x0fffffff) | cN_last_bit;

			uint32_t dN_minus_one = (iter_num_shifts == 2) && (j == 1) ? d0_16[i] : d0_16[i - 1];
			int dN_last_bit = (dN_minus_one >> 27) & 1;
			d0_16[i] = ((dN_minus_one << 1) & 0x0fffffff) | dN_last_bit;
		}
	}

	uint64_t k1_16[16] = {0};

	for (int i = 0; i < 16; i++) {
		uint64_t cN = c0_16[i + 1] & 0xfffffff;
		uint64_t dN = d0_16[i + 1] & 0xfffffff;
		uint64_t cNdN = (cN << 28) | dN;
		for (int j = 0; j < 48; j++) {
			int bit = (cNdN >> (56 - des_pc_table_2[j])) & 1;
			if (bit)
				k1_16[i] |= (1ULL << (47 - j));
		}
	}

	if (decrypt) {
		uint64_t key_copy[16];
		memcpy(key_copy, k1_16, sizeof(key_copy));

		for (int i = 0; i < 16; i++) {
			k1_16[i] = key_copy[15 - i];
		}
	}

	uint64_t msg_ip = 0;

	for (int i = 0; i < 64; i++) {
		int bit = (msg >> (64 - des_ip_table[i])) & 1;
		if (bit)
			msg_ip |= (1ULL << (63 - i));
	}
	
	uint32_t l0 = (msg_ip >> 32) & 0xffffffff;
	uint32_t r0 = msg_ip & 0xffffffff;

	uint32_t l0_16[17];
	uint32_t r0_16[17];
	l0_16[0] = l0;
	r0_16[0] = r0;



	for (int i = 1; i < 17; i++) {
		l0_16[i] = r0_16[i - 1];
		r0_16[i] = l0_16[i - 1] ^ f(r0_16[i - 1], k1_16[i - 1]);
	}

	uint64_t r16_l16 = ((uint64_t)r0_16[16] << 32) | l0_16[16];

	uint64_t final_value = 0;
	for (int i = 0; i < 64; i++) {
		int bit = (r16_l16 >> (64 - des_ip_invert_table[i])) & 1;
		if (bit)
			final_value |= (1ULL << (63 - i));
	}

	return final_value;
}

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage: des-encode <msg> <key>\nNOTE: input your 64-bit message and key in hexadecimal, leading 0x is fine\n");
		return 1;
	}

	char *msg_ptr = argv[1];
	char *key_ptr = argv[2];

	if (msg_ptr[0] == '0' && msg_ptr[1] == 'x') {
		// skip over 0x
		msg_ptr += 2;
	}

	if (key_ptr[0] == '0' && key_ptr[1] == 'x') {
		// skip over 0x
		key_ptr += 2;
	}

	uint64_t msg = strtoull(msg_ptr, NULL, 16);
	if (msg == 0) {
		printf("msg is not valid!\n");
		return 2;
	}

	uint64_t key = strtoull(key_ptr, NULL, 16);
	if (msg == 0) {
		printf("key is not valid!\n");
		return 3;
	}
	uint64_t dec_result = des(msg, key, true);

	printf("Resulting decryption: %" PRIx64 "\n", dec_result);
	return 0;
}