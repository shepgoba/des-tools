#include <stdio.h>
#include <stdint.h>

static const uint8_t des_pc_1[56] = {
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4
};

static const uint8_t des_pc_2[48] = {
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

static const uint8_t des_ls_order[16] = {
	1, 1, 2, 2, 2, 2, 2, 2, 
	1, 2, 2, 2, 2, 2, 2, 1,
};

void printbin32(uint32_t n)
{
	char buf[33];
	for (int i = 0; i < 32; i++) {
		if ((n >> (31 - i)) & 1)
			buf[i] = '1';
		else
			buf[i] = '0';
	}

	buf[32] = '\0';
	printf("%s\n", buf);
}

void printbin64(uint64_t n)
{
	char buf[65];
	for (int i = 0; i < 64; i++) {
		if ((n >> (63 - i)) & 1)
			buf[i] = '1';
		else
			buf[i] = '0';
	}

	buf[64] = '\0';
	printf("%s\n", buf);
}

uint64_t des_enc(uint64_t msg, uint64_t key)
{
	uint64_t k_plus = 0;

	for (int i = 0; i < 56; i++) {
		int bit = (key >> (64 - des_pc_1[i])) & 1;
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

	uint64_t k1_16[16];

	for (int i = 0; i < 16; i++) {
		uint64_t cN = c0_16[i + 1] & 0xfffffff;
		uint64_t dN = d0_16[i + 1] & 0xfffffff;
		uint64_t cNdN = (cN << 28) | dN;
		for (int j = 0; j < 48; j++) {
			int bit = (cNdN >> (56 - des_pc_2[j])) & 1;
			if (bit)
				k1_16[i] |= (1ULL << (47 - j));
		}
		printbin64(k1_16[i]);
	}
	return k_plus;
}

int main(void)
{
	uint64_t msg = 0x0123456789ABCDEF;
	uint64_t key = 0x133457799BBCDFF1;

	uint64_t result = des_enc(msg, key);

	printf("Ciphertext: 0x%llx\n", result);
	return 0;
}