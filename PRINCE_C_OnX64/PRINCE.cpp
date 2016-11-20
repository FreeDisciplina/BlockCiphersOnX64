#include "PRINCE.h"

using namespace std;

u16 Sbox[16] = {0xB, 0xF, 0x3, 0x2, 0xA, 0xC, 0x9, 0x1, 0x6, 0x7, 0x8, 0x0, 0xE, 0x5, 0xD, 0x4 };
u16 Sbox4[1<<16];

u16 iSbox[16];
u16 iSbox4[1<<16];

u16 RC[Round_n][4] = 
{
	/*RC0 */ {0x0000, 0x0000, 0x0000, 0x0000, }, 
	/*RC1 */ {0x7344, 0x0370, 0x8a2e, 0x1319, }, 
	/*RC2 */ {0x31d0, 0x299f, 0x3822, 0xa409, }, 
	/*RC3 */ {0x6c89, 0xec4e, 0xfa98, 0x082e, }, 
	/*RC4 */ {0x1377, 0x38d0, 0x21e6, 0x4528, }, 
	/*RC5 */ {0x0c6c, 0x34e9, 0x66cf, 0xbe54, }, 
	/*RC6 */ {0x5cb1, 0xfd95, 0x4f78, 0x7ef8, }, 
	/*RC7 */ {0x43aa, 0xf1ac, 0x0851, 0x8584, }, 
	/*RC8 */ {0x3c54, 0x2532, 0xd32f, 0xc882, }, 
	/*RC9 */ {0x610d, 0xe0e3, 0x1195, 0x64a5, }, 
	/*RC10*/ {0x2399, 0xca0c, 0xa399, 0xd3b5, }, 
	/*RC11*/ {0x50dd, 0xc97c, 0x29b7, 0xc0ac, }, 
};

#define PRINT_Table 1

void GenSbox4()
{
	for (u16 i = 0; i < 16; i++)
	{
		iSbox[Sbox[i]] = i;
	}

	u16 y = 0;
	for (u16 i = 0; i <= 0xffff; i++)
	{
		y = 
			(Sbox[(i>>0x0)&0xf]<<0x0) |
			(Sbox[(i>>0x4)&0xf]<<0x4) |
			(Sbox[(i>>0x8)&0xf]<<0x8) |
			(Sbox[(i>>0xc)&0xf]<<0xc);
		Sbox4[i] = y;

		y = 
			(iSbox[(i>>0x0)&0xf]<<0x0) |
			(iSbox[(i>>0x4)&0xf]<<0x4) |
			(iSbox[(i>>0x8)&0xf]<<0x8) |
			(iSbox[(i>>0xc)&0xf]<<0xc);
		iSbox4[i] = y;

		if (i==0xffff) break;
	}

	for (u16 i = 0; i < 16; i++)
	{
		iSbox[Sbox[i]] = i;
	}

#if (PRINT_Table==1)
	ofstream fout;
	fout.open("Sbox4.txt");
	fout << "#include \"PRINCE.h\"" << endl << endl;

	fout << "u16 Sbox4[1<<16] = {" << endl;
	fout << hex << setfill('0');
	for (u16 i = 0; i <= 0xffff; i++)
	{
		fout << "0x" << setw(4) << Sbox4[i] << ", ";
		if (i==0xffff) break;
	}
	fout << dec << setfill(' ');
	fout << endl << endl;
	fout << "u16 iSbox4[1<<16] = {" << endl;
	fout << hex << setfill('0');
	for (u16 i = 0; i <= 0xffff; i++)
	{
		fout << "0x" << setw(4) << iSbox4[i] << ", ";
		if (i==0xffff) break;
	}
	fout << dec << setfill(' ');
	fout << endl;

	fout.close();
#endif

}

void encrypt(u8 *input, u8 *output, u8 *key)
{
	u8  s[8];

	u16 s0, s1, s2, s3;
	u16 t0, t1, t2, t3;

	u16 *ip = (u16 *)(input);
	u16 *op = (u16 *)(output);

	u16 *wkp = (u16 *)key;
	u16 *rkp = (u16 *)(key + 8);

	u64 *wkp_64 = (u64 *)key;
	u64 wk_64 = *wkp_64;

	u64 wk_tmp = *wkp_64;

	GenSbox4();

	input(s0, s1, s2, s3);
	*wkp_64 = ((wk_64 >> 1) | (wk_64 << 63)) ^ (wk_64 >> 63);

	ARK(s0, s1, s2, s3);
	ARC(s0, s1, s2, s3, 0);

	R1_5(s0, s1, s2, s3, 1);
	R1_5(s0, s1, s2, s3, 2);
	R1_5(s0, s1, s2, s3, 3);
	R1_5(s0, s1, s2, s3, 4);
	R1_5(s0, s1, s2, s3, 5);

	S(s0, s1, s2, s3);
	M_prime(s0, s1, s2, s3);
	iS(s0, s1, s2, s3);

	R6_10(s0, s1, s2, s3, 6);
	R6_10(s0, s1, s2, s3, 7);
	R6_10(s0, s1, s2, s3, 8);
	R6_10(s0, s1, s2, s3, 9);
	R6_10(s0, s1, s2, s3, 10);

	ARC(s0, s1, s2, s3, 11);
	ARK(s0, s1, s2, s3);

	output(s0, s1, s2, s3);
	*wkp_64 = wk_tmp;
}


void decrypt(u8 *input, u8 *output, u8 *key)
{
	u8  s[8];

	u16 s0, s1, s2, s3;
	u16 t0, t1, t2, t3;

	u16 *ip = (u16 *)(input);
	u16 *op = (u16 *)(output);

	u16 *wkp = (u16 *)key;
	u16 *rkp = (u16 *)(key + 8);

	u64 *wkp_64 = (u64 *)key;
	u64 wk_64 = *wkp_64;

	u64 wk_tmp = *wkp_64;

	GenSbox4();

	*wkp_64 = ((wk_64 >> 1) | (wk_64 << 63)) ^ (wk_64 >> 63);
	input(s0, s1, s2, s3);
	*wkp_64 = wk_tmp;

	u64 alpha = 0xc0ac29b7c97c50ddull;
	*(u64 *)rkp = *(u64 *)rkp ^ alpha;

	ARK(s0, s1, s2, s3);
	ARC(s0, s1, s2, s3, 0);

	R1_5(s0, s1, s2, s3, 1);
	R1_5(s0, s1, s2, s3, 2);
	R1_5(s0, s1, s2, s3, 3);
	R1_5(s0, s1, s2, s3, 4);
	R1_5(s0, s1, s2, s3, 5);

	S(s0, s1, s2, s3);
	M_prime(s0, s1, s2, s3);
	iS(s0, s1, s2, s3);

	R6_10(s0, s1, s2, s3, 6);
	R6_10(s0, s1, s2, s3, 7);
	R6_10(s0, s1, s2, s3, 8);
	R6_10(s0, s1, s2, s3, 9);
	R6_10(s0, s1, s2, s3, 10);

	ARC(s0, s1, s2, s3, 11);
	ARK(s0, s1, s2, s3);

	output(s0, s1, s2, s3);
	*(u64 *)rkp = *(u64 *)rkp ^ alpha;
}


void encrypt_CBC(u8 *iv, u8 *in, u8 *out, u8 *Key, int blockn)
{
	u8 state[BLOCK_SIZE_INBYTE];
	for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
	{
		state[si] = iv[si];
	}

	for (int bi = 0; bi < blockn; bi++)
	{
		for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
		{
			state[si] ^= in[bi*BLOCK_SIZE_INBYTE + si];
		}
		encrypt(state, state, Key);
		for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
		{
			out[bi*BLOCK_SIZE_INBYTE + si] = state[si];
		}
	}
}


void decrypt_CBC(u8 *iv, u8 *in, u8 *out, u8 *Key, int blockn)
{
	u8 state[BLOCK_SIZE_INBYTE];
	for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
	{
		state[si] = in[si];
	}
	decrypt(state, state, Key);
	for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
	{
		out[si] = iv[si] ^ state[si];
	}

	for (int bi = 1; bi < blockn; bi++)
	{
		for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
		{
			state[si] = in[bi*BLOCK_SIZE_INBYTE + si];
		}
		decrypt(state, state, Key);
		for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
		{
			out[bi*BLOCK_SIZE_INBYTE + si] = in[(bi-1)*BLOCK_SIZE_INBYTE + si] ^ state[si];
		}
	}
}


void encrypt_CTR(u8 *ct, u8 *in, u8 *out, u8 *Key, int blockn)
{
	u8 state[BLOCK_SIZE_INBYTE];
	for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
	{
		state[si] = ct[si];
	}

	for (int bi = 0; bi < blockn; bi++)
	{
		encrypt(state, state, Key);
		for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
		{
			out[bi*BLOCK_SIZE_INBYTE + si] = in[bi*BLOCK_SIZE_INBYTE + si] ^ state[si];
		}
		ct[BLOCK_SIZE_INBYTE - 1]++;
		for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
		{
			state[si] = ct[si];
		}
	}
}


void decrypt_CTR(u8 *ct, u8 *in, u8 *out, u8 *Key, int blockn)
{
	u8 state[BLOCK_SIZE_INBYTE];
	for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
	{
		state[si] = ct[si];
	}

	for (int bi = 0; bi < blockn; bi++)
	{
		encrypt(state, state, Key);
		for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
		{
			out[bi*BLOCK_SIZE_INBYTE + si] = in[bi*BLOCK_SIZE_INBYTE + si] ^ state[si];
		}
		ct[BLOCK_SIZE_INBYTE - 1]++;
		for (int si = 0; si < BLOCK_SIZE_INBYTE; si++)
		{
			state[si] = ct[si];
		}
	}
}


