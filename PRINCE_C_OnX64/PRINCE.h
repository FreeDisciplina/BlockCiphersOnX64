#ifndef PRINCE_H__
#define PRINCE_H__
#include "types.h"

#define Round_n 12
#define BLOCK_SIZE_INBYTE 8


extern u16 RC[Round_n][4];
extern u16 Sbox[16];
extern u16 Sbox4[1<<16];

extern u16 iSbox[16];
extern u16 iSbox4[1<<16];

void GenSbox4();

/*

M0	
	0000
	0100
	0010
	0001
M1
	1000
	0000
	0010
	0001

M2
	1000
	0100
	0000
	0001
M3
	1000
	0100
	0010
	0000

M^(0)
M0 M1 M2 M3
M1 M2 M3 M0
M2 M3 M0 M1
M3 M0 M1 M2


0000 1000 1000 1000 =     4 ^ 8 ^ c		=   ^ b ^ 7 ^ 3
0100 0000 0100 0100 = 1     ^ 9 ^ d		= e	^   ^ 6 ^ 2
0010 0010 0000 0010 = 2 ^ 6 ^   ^ e		= d	^ 9 ^   ^ 1
0001 0001 0001 0000 = 3 ^ 7 ^ b			= c	^ 8 ^ 4 ^  

1000 1000 1000 0000 = 0 ^ 4 ^ 8			= f ^ b ^ 7 ^      
0000 0100 0100 0100 =   ^ 5 ^ 9 ^ d		=   ^ a ^ 6 ^ 2 
0010 0000 0010 0010 = 2 ^   ^ a ^ e		= d ^   ^ 5 ^ 1 
0001 0001 0000 0001 = 3 ^ 7 ^   ^ f		= c ^ 8 ^   ^ 0 

1000 1000 0000 1000 = 0 ^ 4 ^   ^ c		= f	^ b ^   ^ 3 
0100 0100 0100 0000 = 1 ^ 5 ^ 9 ^		= e	^ a ^ 6 ^   
0000 0010 0010 0010 =   ^ 6 ^ a ^ e		=  	^ 9 ^ 5 ^ 1 
0001 0000 0001 0001 = 3 ^   ^ b ^ f		= c	^   ^ 4 ^ 0 

1000 0000 1000 1000 = 0 ^   ^ 8 ^ c		= f	^   ^ 7 ^ 3 
0100 0100 0000 0100 = 1 ^ 5 ^   ^ d		= e	^ a ^   ^ 2 
0010 0010 0010 0000 = 2 ^ 6 ^ a ^		= d	^ 9 ^ 5 ^   
0000 0001 0001 0001 =   ^ 7 ^ b ^ f		=  	^ 8 ^ 4 ^ 0 

M^(1)
M1 M2 M3 M0 
M2 M3 M0 M1 
M3 M0 M1 M2 
M0 M1 M2 M3 


=

1000 1000 1000 0000 = 0 4 8 
0000 0100 0100 0100 =   5 9 d
0010 0000 0010 0010 = 2   a e
0001 0001 0000 0001 = 3 7   f

1000 1000 0000 1000 = 0 4   c  
0100 0100 0100 0000 = 1 5 9 
0000 0010 0010 0010 =   6 a e 
0001 0000 0001 0001 = 3   b f  

1000 0000 1000 1000 = 0   8 c
0100 0100 0000 0100 = 1 5   d
0010 0010 0010 0000 = 2 6 a 
0000 0001 0001 0001 =   7 b f

0000 1000 1000 1000 =   4 8 c
0100 0000 0100 0100 = 1   9 d
0010 0010 0000 0010 = 2 6   e
0001 0001 0001 0000 = 3 7 b

M = M^(0) M^(1) M^(1) M^(0)
*/

#define Output(x0, x1, x2, x3) \
{ \
	fout_hex << hex  << setfill('0') << "  " << "$" << setw(2) << s[0]+'\0' << ","; \
	fout_hex << hex  << setfill('0') << "  " << "$" << setw(2) << s[1]+'\0' << ","; \
	fout_hex << hex  << setfill('0') << "  " << "$" << setw(2) << s[2]+'\0' << ","; \
	fout_hex << hex  << setfill('0') << "  " << "$" << setw(2) << s[3]+'\0' << ","; \
	fout_hex << hex  << setfill('0') << "  " << "$" << setw(2) << s[4]+'\0' << ","; \
	fout_hex << hex  << setfill('0') << "  " << "$" << setw(2) << s[5]+'\0' << ","; \
	fout_hex << hex  << setfill('0') << "  " << "$" << setw(2) << s[6]+'\0' << ","; \
	fout_hex << hex  << setfill('0') << "  " << "$" << setw(2) << s[7]+'\0' << ","; \
	fout_hex << endl; \
}

#  define word16_in(x,n)    (*((u16*)(x)+(n)))
#  define word16_out(x,n,v) (*((u16*)(x)+(n)) = (v))

#define input(x0, x1, x2, x3) \
	{ \
	x0 = word16_in(ip, 0) ^ (wkp)[0]; \
	x1 = word16_in(ip, 1) ^ (wkp)[1]; \
	x2 = word16_in(ip, 2) ^ (wkp)[2]; \
	x3 = word16_in(ip, 3) ^ (wkp)[3]; \
};

#define output(x0, x1, x2, x3) \
	{ \
	word16_out(op, 0, x0 ^ (wkp)[0]); \
	word16_out(op, 1, x1 ^ (wkp)[1]); \
	word16_out(op, 2, x2 ^ (wkp)[2]); \
	word16_out(op, 3, x3 ^ (wkp)[3]); \
};

#define S(x0, x1, x2, x3) \
{ \
	x0 = Sbox4[x0]; \
	x1 = Sbox4[x1]; \
	x2 = Sbox4[x2]; \
	x3 = Sbox4[x3]; \
}

#define iS(x0, x1, x2, x3) \
{ \
	x0 = iSbox4[x0]; \
	x1 = iSbox4[x1]; \
	x2 = iSbox4[x2]; \
	x3 = iSbox4[x3]; \
}


#define M0(x) \
{ \
	t0 = (((x >> 0x0)&0xf)<<0x0) | (((x >> 0x0)&0xf)<<0x4) | (((x >> 0x0)&0xf)<<0x8) | (((x >> 0x0)&0xf)<<0xc); \
	t1 = (((x >> 0x4)&0xf)<<0x0) | (((x >> 0x4)&0xf)<<0x4) | (((x >> 0x4)&0xf)<<0x8) | (((x >> 0x4)&0xf)<<0xc); \
	t2 = (((x >> 0x8)&0xf)<<0x0) | (((x >> 0x8)&0xf)<<0x4) | (((x >> 0x8)&0xf)<<0x8) | (((x >> 0x8)&0xf)<<0xc); \
	t3 = (((x >> 0xc)&0xf)<<0x0) | (((x >> 0xc)&0xf)<<0x4) | (((x >> 0xc)&0xf)<<0x8) | (((x >> 0xc)&0xf)<<0xc); \
	t0 = t0 & 0xe7bd; \
	t1 = t1 & 0xde7b; \
	t2 = t2 & 0xbde7; \
	t3 = t3 & 0x7bde; \
	x = t0 ^ t1 ^ t2 ^ t3; \
}

#define M1(x) \
{ \
	t0 = (((x >> 0x0)&0xf)<<0x0) | (((x >> 0x0)&0xf)<<0x4) | (((x >>0x0)&0xf)<<0x8) | (((x >> 0x0)&0xf)<<0xc); \
	t1 = (((x >> 0x4)&0xf)<<0x0) | (((x >> 0x4)&0xf)<<0x4) | (((x >>0x4)&0xf)<<0x8) | (((x >> 0x4)&0xf)<<0xc); \
	t2 = (((x >> 0x8)&0xf)<<0x0) | (((x >> 0x8)&0xf)<<0x4) | (((x >>0x8)&0xf)<<0x8) | (((x >> 0x8)&0xf)<<0xc); \
	t3 = (((x >> 0xc)&0xf)<<0x0) | (((x >> 0xc)&0xf)<<0x4) | (((x >>0xc)&0xf)<<0x8) | (((x >> 0xc)&0xf)<<0xc); \
	t0 = t0 & 0x7bde; \
	t1 = t1 & 0xe7bd; \
	t2 = t2 & 0xde7b; \
	t3 = t3 & 0xbde7; \
	x = t0 ^ t1 ^ t2 ^ t3; \
}

#define SR(x0, x1, x2, x3) \
{ \
	t0 = (x1 & 0xf) | (x2 & 0xf0) | (x3 & 0xf00) | (x0 & 0xf000); \
	t1 = (x2 & 0xf) | (x3 & 0xf0) | (x0 & 0xf00) | (x1 & 0xf000); \
	t2 = (x3 & 0xf) | (x0 & 0xf0) | (x1 & 0xf00) | (x2 & 0xf000); \
	t3 = (x0 & 0xf) | (x1 & 0xf0) | (x2 & 0xf00) | (x3 & 0xf000); \
	x0 = t0; \
	x1 = t1; \
	x2 = t2; \
	x3 = t3; \
}

#define iSR(x0, x1, x2, x3) \
{ \
	t0 = (x3 & 0xf) | (x2 & 0xf0) | (x1 & 0xf00) | (x0 & 0xf000); \
	t1 = (x0 & 0xf) | (x3 & 0xf0) | (x2 & 0xf00) | (x1 & 0xf000); \
	t2 = (x1 & 0xf) | (x0 & 0xf0) | (x3 & 0xf00) | (x2 & 0xf000); \
	t3 = (x2 & 0xf) | (x1 & 0xf0) | (x0 & 0xf00) | (x3 & 0xf000); \
	x0 = t0; \
	x1 = t1; \
	x2 = t2; \
	x3 = t3; \
}

#define M_prime(x0, x1, x2, x3) \
{ \
	M0(x0); \
	M1(x1); \
	M1(x2); \
	M0(x3); \
}


#define M(x0, x1, x2, x3) \
{ \
	M_prime(x0, x1, x2, x3); \
	SR(x0, x1, x2, x3); \
}

#define iM(x0, x1, x2, x3) \
{ \
	iSR(x0, x1, x2, x3); \
	M_prime(x0, x1, x2, x3); \
}

#define ARC(x0, x1, x2, x3, r) \
{ \
	x0 ^= RC[r][0]; \
	x1 ^= RC[r][1]; \
	x2 ^= RC[r][2]; \
	x3 ^= RC[r][3]; \
}

#define ARK(x0, x1, x2, x3) \
{ \
	x0 ^= (rkp)[0]; \
	x1 ^= (rkp)[1]; \
	x2 ^= (rkp)[2]; \
	x3 ^= (rkp)[3]; \
}

#define R1_5(x0, x1, x2, x3, r) \
{ \
	S(x0, x1, x2, x3); \
	M(x0, x1, x2, x3); \
	ARC(x0, x1, x2, x3, r); \
	ARK(x0, x1, x2, x3); \
}

#define R6_10(x0, x1, x2, x3, r) \
{ \
	ARK(x0, x1, x2, x3); \
	ARC(x0, x1, x2, x3, r); \
	iM(x0, x1, x2, x3); \
	iS(x0, x1, x2, x3); \
}

void encrypt(u8 *in, u8 *out, u8 *Key);
void decrypt(u8 *in, u8 *out, u8 *Key);

void encrypt_CBC(u8 *iv, u8 *in, u8 *out, u8 *Key, int blockn);
void decrypt_CBC(u8 *iv, u8 *in, u8 *out, u8 *Key, int blockn);

void encrypt_CTR(u8 *ct, u8 *in, u8 *out, u8 *Key, int blockn);
void decrypt_CTR(u8 *ct, u8 *in, u8 *out, u8 *Key, int blockn);

#endif //PRINCE_H__