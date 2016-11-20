#include "PRINCE.h"
#include "test.h"
#include <bitset>

using namespace std;

#define word16_in(x,n)    (*((u16*)(x)+(n)))

#define word32_in(x, n) (*(((u32*)(x))+ n))

void test()
{
	#define VN 5

	u8 test_vector_plain[VN][8] = 
	{
		//{0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
		{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,},
		{0xef,0xcd,0xab,0x89,0x67,0x45,0x23,0x01,},
		//{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,},
	};

	u8 test_vector_key128[VN][16] = 
	{
		{0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,}, 
		{0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,}, 
		{0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,}, 
		{0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,}, 
		{0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x10,0x32,0x54,0x76, 0x98,0xba,0xdc,0xfe,}, 
	};

	u8 test_plain[VN][8];
	u8 test_cipher[VN][8];

	ofstream fout_hex, fout_bit;
	string fn_hex, fn_bit;

	fn_hex = "test_vector_hex.txt";
	fn_bit = "text_vector_bit.txt";
	fout_hex.open(fn_hex.c_str());
	fout_bit.open(fn_bit.c_str());

	fout_hex << "Enc K128:" << endl;
	fout_hex << setw(20) << "plaintext" << setw(36) << "key" << setw(20) << "ciphertext" << endl;
	fout_bit << "Enc K128:" << endl;
	fout_bit << setw(20) << "plaintext" << setw(36) << "key" << setw(20) << "ciphertext" << endl;
	for (int i=0; i<VN; i++)
	{
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)test_vector_plain[i])[0];
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)test_vector_key128[i])[1] << setw(16) << ((u64*)(test_vector_key128[i]))[0];
		encrypt(test_vector_plain[i], test_cipher[i], test_vector_key128[i]);
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)test_cipher[i])[0];
		fout_hex << setfill(' ') << dec << endl;
	
		fout_bit << "    " << setw(16) << *(new bitset<16>(word16_in(test_vector_plain[i], 0)))
			<< "    " << setw(32) << *(new bitset<32>((u64)word32_in(test_vector_key128[i], 0)))
			<< "    " << setw(16) << *(new bitset<16>(word16_in(test_cipher[i], 0))) << endl;
		fout_bit << "    " << setw(16) << *(new bitset<16>(word16_in(test_vector_plain[i], 1)))
			<< "    " << setw(32) << *(new bitset<32>((u64)word32_in(test_vector_key128[i], 1)))
			<< "    " << setw(16) << *(new bitset<16>(word16_in(test_cipher[i], 1))) << endl;
		fout_bit << "    " << setw(16) << *(new bitset<16>(word16_in(test_vector_plain[i], 2)))
			<< "    " << setw(32) << *(new bitset<32>((u64)word32_in(test_vector_key128[i], 2)))
			<< "    " << setw(16) << *(new bitset<16>(word16_in(test_cipher[i], 2))) << endl;
		fout_bit << "    " << setw(16) << *(new bitset<16>(word16_in(test_vector_plain[i], 3)))
			<< "    " << setw(32) << *(new bitset<32>((u64)word32_in(test_vector_key128[i], 3)))
			<< "    " << setw(16) << *(new bitset<16>((u64)word16_in(test_cipher[i], 3))) << endl;
		fout_bit << endl;
	}
	fout_hex << "Dec K128:" << endl;
	fout_hex << setw(20) << "plaintext" << setw(36) << "key" << setw(20) << "ciphertext" << endl;
	fout_bit << "Dec K128:" << endl;
	fout_bit << setw(20) << "plaintext" << setw(36) << "key" << setw(20) << "ciphertext" << endl;
	for (int i=0; i<VN; i++)
	{
		decrypt(test_cipher[i], test_plain[i],  test_vector_key128[i]);
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)test_plain[i])[0];
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)test_vector_key128[i])[1] << setw(16) << ((u64*)(test_vector_key128[i]))[0];
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)test_cipher[i])[0];
		fout_hex << setfill(' ') << dec << endl;

		fout_bit << "    " << setw(16) << *(new bitset<16>(word16_in(test_plain[i], 0)))
			<< "    " << setw(32) << *(new bitset<32>((u64)word32_in(test_vector_key128[i], 0)))
			<< "    " << setw(16) << *(new bitset<16>(word16_in(test_cipher[i], 0))) << endl;
		fout_bit << "    " << setw(16) << *(new bitset<16>(word16_in(test_plain[i], 1)))
			<< "    " << setw(32) << *(new bitset<32>((u64)word32_in(test_vector_key128[i], 1)))
			<< "    " << setw(16) << *(new bitset<16>(word16_in(test_cipher[i], 1))) << endl;
		fout_bit << "    " << setw(16) << *(new bitset<16>(word16_in(test_plain[i], 2)))
			<< "    " << setw(32) << *(new bitset<32>((u64)word32_in(test_vector_key128[i], 2)))
			<< "    " << setw(16) << *(new bitset<16>(word16_in(test_cipher[i], 2))) << endl;
		fout_bit << "    " << setw(16) << *(new bitset<16>(word16_in(test_plain[i], 3)))
			<< "    " << setw(32) << *(new bitset<32>((u64)word32_in(test_vector_key128[i], 3)))
			<< "    " << setw(16) << *(new bitset<16>(word16_in(test_cipher[i], 3))) << endl;
		fout_bit << endl;
	}

	fout_hex << endl;
	fout_hex.close();
	fout_bit << endl;
	fout_bit.close();
}

void test_mode()
{
#define CBC_BLOCKS 16
#define INITV_NUM_BYTE BLOCK_SIZE_INBYTE
#define PTEXT_NUM_BYTE (CBC_BLOCKS*BLOCK_SIZE_INBYTE)
#define KEY0_NUM_BYTE BLOCK_SIZE_INBYTE
#define KEY1_NUM_BYTE BLOCK_SIZE_INBYTE
#define KEY_NUM_BYTE (KEY0_NUM_BYTE + KEY1_NUM_BYTE)

	u8 CBC_initv[BLOCK_SIZE_INBYTE];
	u8 CBC_input[CBC_BLOCKS*BLOCK_SIZE_INBYTE];
	u8 CBC_output[CBC_BLOCKS*BLOCK_SIZE_INBYTE];
	u8 CBC_key[KEY_NUM_BYTE];
	int i;
	for (i = INITV_NUM_BYTE; i > 0; i--)
	{
		CBC_initv[INITV_NUM_BYTE - i] = i;
	}
	for (i = PTEXT_NUM_BYTE; i > 0; i--)
	{
		CBC_input[PTEXT_NUM_BYTE - i] = i;
	}
	for (i = KEY0_NUM_BYTE; i > 0; i--)
	{
		CBC_key[KEY0_NUM_BYTE - i] = i;
	}
	for (i = KEY1_NUM_BYTE; i > 0; i--)
	{
		CBC_key[KEY0_NUM_BYTE + KEY1_NUM_BYTE - i] = i;
	}

	ofstream fout_hex;
	string fn_hex;

	fn_hex = "test_mode_cbc_vector_hex.txt";
	fout_hex.open(fn_hex.c_str());

	encrypt_CBC(CBC_initv, CBC_input, CBC_output, CBC_key, CBC_BLOCKS);

	fout_hex << "Enc K128:" << endl;
	fout_hex << setw(20) << "plaintext" << setw(36) << "key" << setw(20) << "ciphertext" << endl;
	for (int i=0; i<CBC_BLOCKS; i++)
	{
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CBC_input)[i];
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CBC_key)[1] << setw(16) << ((u64*)(CBC_key))[0];
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CBC_output)[i];
		fout_hex << setfill(' ') << dec << endl;
	}

	decrypt_CBC(CBC_initv, CBC_input, CBC_output, CBC_key, CBC_BLOCKS);
	fout_hex << "Dec K128:" << endl;
	fout_hex << setw(20) << "plaintext" << setw(36) << "key" << setw(20) << "ciphertext" << endl;
	for (int i=0; i<CBC_BLOCKS; i++)
	{
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CBC_input)[i];
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CBC_key)[1] << setw(16) << ((u64*)(CBC_key))[0];
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CBC_output)[i];
		fout_hex << setfill(' ') << dec << endl;
	}

	fout_hex << endl;
	fout_hex.close();


#undef PTEXT_NUM_BYTE (CBC_BLOCKS*BLOCK_SIZE_INBYTE)
#define CTR_BLOCKS 2
#define COUNT_NUM_BYTE BLOCK_SIZE_INBYTE
#define PTEXT_NUM_BYTE (CTR_BLOCKS*BLOCK_SIZE_INBYTE)

	u8 CTR_count[BLOCK_SIZE_INBYTE];
	u8 CTR_input[CBC_BLOCKS*BLOCK_SIZE_INBYTE];
	u8 CTR_output[CBC_BLOCKS*BLOCK_SIZE_INBYTE];
	u8 CTR_key[KEY_NUM_BYTE];

	for (i = COUNT_NUM_BYTE; i > 0; i--)
	{
		CTR_count[COUNT_NUM_BYTE - i] = i;
	}
	for (i = PTEXT_NUM_BYTE; i > 0; i--)
	{
		CTR_input[PTEXT_NUM_BYTE - i] = i;
	}
	for (i = KEY0_NUM_BYTE; i > 0; i--)
	{
		CTR_key[KEY0_NUM_BYTE - i] = 0;
	}
	for (i = KEY1_NUM_BYTE; i > 0; i--)
	{
		CTR_key[KEY0_NUM_BYTE + KEY1_NUM_BYTE - i] = 0;
	}

	fn_hex = "test_mode_ctr_vector_hex.txt";
	fout_hex.open(fn_hex.c_str());

	encrypt_CTR(CTR_count, CTR_input, CTR_output, CTR_key, CTR_BLOCKS);

	fout_hex << "Enc K128:" << endl;
	fout_hex << setw(20) << "plaintext" << setw(36) << "key" << setw(20) << "ciphertext" << endl;
	for (int i=0; i<CTR_BLOCKS; i++)
	{
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CTR_input)[i];
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CTR_key)[1] << setw(16) << ((u64*)(CTR_key))[0];
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CTR_output)[i];
		fout_hex << setfill(' ') << dec << endl;
	}

	for (i = COUNT_NUM_BYTE; i > 0; i--)
	{
		CTR_count[COUNT_NUM_BYTE - i] = i;
	}

	decrypt_CTR(CTR_count, CTR_input, CTR_output, CTR_key, CTR_BLOCKS);
	fout_hex << "Dec K128:" << endl;
	fout_hex << setw(20) << "plaintext" << setw(36) << "key" << setw(20) << "ciphertext" << endl;
	for (int i=0; i<CTR_BLOCKS; i++)
	{
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CTR_input)[i];
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CTR_key)[1] << setw(16) << ((u64*)(CTR_key))[0];
		fout_hex << "  0x" << setw(16) << hex << setfill('0') << ((u64*)CTR_output)[i];
		fout_hex << setfill(' ') << dec << endl;
	}

	fout_hex << endl;
	fout_hex.close();
}
