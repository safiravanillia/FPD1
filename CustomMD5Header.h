#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"


typedef unsigned char byte;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef struct {
    uint32 state[4];    // wordA, wordB, wordC, wordD
}md5context_t;

// memcpy exists to placate GCC.  Use memmove.
void*
memcpy(void *dst, void *src, uint n)
{
  return memmove(dst, src, n);
}

#define WORD_A 0x67452301
#define WORD_B 0xefcdab89
#define WORD_C 0x98badcfe
#define WORD_D 0x10325476

uint32 Kmap[64] =   // floor(abs(sin(i+1))) * pow(2,32); i=0...63
{
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

uint32 rotate[]=
{
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9 ,14, 20, 5, 9 ,14, 20, 5, 9 ,14, 20, 5, 9 ,14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

uint32 F(uint32 x, uint32 y, uint32 z){
    return ((x & y) | ((~x) & z));
}

uint32 G(uint32 x, uint32 y, uint32 z){
    return ((x & z) | (y & (~z)));
}

uint32 H(uint32 x, uint32 y, uint32 z){
    return (x ^ y ^ z);
}

uint32 I(uint32 x, uint32 y, uint32 z){
    return (y ^ (x | (~z)));
}

uint32 LROT(uint32 integer, uint32 n_shift){
    return ((integer << n_shift) | (integer >> (32-n_shift)));
}

void FF(uint32 * A, uint32 B, uint32 C, uint32 D,uint32 x, uint32 s, uint32 t){
    *A += F(B, C, D) + x + (uint32)(t);
    *A = LROT(*A, s);
    *A += B;
  //  printf("%x\n",*A);
}

void GG(uint32 * A, uint32 B, uint32 C, uint32 D,uint32 x, uint32 s, uint32 t){
    *A += G(B, C, D) + x + (uint32)(t);
    *A = LROT(*A, s);
    *A += B;
    // printf("%x %x %x\n",B, C, D);
}

void HH(uint32 * A, uint32 B, uint32 C, uint32 D,uint32 x, uint32 s, uint32 t){
    *A += H(B, C, D) + x + (uint32)(t);
    *A = LROT(*A, s);
    *A += B;
    // printf("%x %x %x\n",B, C, D);
}

void II(uint32 * A, uint32 B, uint32 C, uint32 D,uint32 x, uint32 s, uint32 t){
    *A += I(B, C, D) + x + (uint32)(t);
    *A = LROT(*A, s);
    *A += B;
    // printf("%x %x %x\n",B, C, D);
}

void Init_Context(md5context_t * ctx){
    ctx->state[0] = (uint32)WORD_A;
    ctx->state[1] = (uint32)WORD_B;
    ctx->state[2] = (uint32)WORD_C;
    ctx->state[3] = (uint32)WORD_D;
}

void Decode(uint32 * dst, byte * src, uint64 len){
    uint64 i,j;
    for(i=0, j=0; j<len; i++, j+=4){
        dst[i] = ((uint32)src[j] | (uint32)(src[j+1]<<8) | 
                    (uint32)(src[j+2]<<16) | (uint32)(src[j+3]<<24));
    }
}

void Encode(uint32 * src, byte * dst){
    int i;
    for(i=0; i<16; i++){
        dst[i] = (byte)((src[(i/4)] >> ((i%4)*8))&0x000000ff);
    }
}

void Transform(md5context_t * ctx, byte * msg){
    uint32 XX[4] = {
        ctx->state[0], ctx->state[1], ctx->state[2], ctx->state[3]
    };
    uint32 X[16];
    Decode(X, msg, 64);
    uint64 i;
    //Round 1
    for(i=0; i<16; i++){
        FF(&XX[(16-i)%4], XX[(17-i)%4], XX[(18-i)%4], XX[(19-i)%4], X[i], rotate[i], Kmap[i]);
     //   printf("%x %x %x %x\n",XX[(15-i)%4], XX[(16-i)%4], XX[(17-i)%4], XX[(18-i)%4]);
    }
    //Round 2
    for(i=0; i<16; i++){
        GG(&XX[(16-i)%4], XX[(17-i)%4], XX[(18-i)%4], XX[(19-i)%4], X[(1+(i)*5)%16], rotate[i+16], Kmap[i+16]);
     //   printf("%x %x %x %x\n",XX[(15-i)%4], XX[(16-i)%4], XX[(17-i)%4], XX[(18-i)%4]);
    }
    //Round 3
    for(i=0; i<16; i++){
        HH(&XX[(16-i)%4], XX[(17-i)%4], XX[(18-i)%4], XX[(19-i)%4], X[(5+(i)*3)%16], rotate[i+32], Kmap[i+32]);
      //  printf("%x %x %x %x\n",XX[(15-i)%4], XX[(16-i)%4], XX[(17-i)%4], XX[(18-i)%4]);
    }
    //Round 4
    for(i=0; i<16; i++){
        II(&XX[(16-i)%4], XX[(17-i)%4], XX[(18-i)%4], XX[(19-i)%4], X[((i)*7)%16], rotate[i+48], Kmap[i+48]);
      //  printf("%x %x %x %x\n",XX[(15-i)%4], XX[(16-i)%4], XX[(17-i)%4], XX[(18-i)%4]);
    }
    ctx->state[0] += XX[0];
    ctx->state[1] += XX[1];
    ctx->state[2] += XX[2];
    ctx->state[3] += XX[3];
}

void Update_Context(md5context_t * ctx, byte * input, uint64 len){
    uint64 newlength = len + (64-len%64);
    byte * input_pad = (byte *)malloc((uint)newlength);
    memcpy(input_pad,input,len);
    uint64 i;
    for(i=len+1; i<newlength; i++) input_pad[i] = (byte)0x0;
    input_pad[len] = (byte)0x80;
    len *= 8;
    memcpy(input_pad+newlength-8,&len, 1);
    // for(i=0; i<newlength; i++)
    //     printf("%d ",input_pad[i]);
    // printf("\n");
    for(i=0; i<newlength; i+=64)
        Transform(ctx, input_pad);
    free(input_pad);
}

void Finalization(md5context_t * ctx, byte * hash){
    Encode(ctx->state, hash);
}

void printmsg(unsigned char *msg,unsigned int len)
{
	unsigned int i;
	const char base16[16] = {
		'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	for(i=0;i<len;i++)
	{
		printf(1,"%c",base16[(((int)msg[i]&0xf0)>>4)]);
		printf(1,"%c",base16[(((int)msg[i]&0xf))]);
	}
	printf(1,"\n");
}
