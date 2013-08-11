// Pr�ctica 1 de Sistemes Operatius
// llibreria
// md5c.c -- from /usr/src/lib/libmd/md5c.c, FreeBSD 6.2
// 27 NOV 2007 -- 19:29



//#include <machine/endian.h>   // <=== suprimit
//#include <sys/endian.h>       // <=== suprimit
#include "md5.h"		// <=== afegit
//#include <sys/md5.h>          // <=== suprimit

static void MD5Transform (u_int32_t[4], const unsigned char[64]);

#ifdef _KERNEL
#define memset(x,y,z)	bzero(x,z);
#define memcpy(x,y,z)	bcopy(y, x, z)
#endif

#if (BYTE_ORDER == LITTLE_ENDIAN)
#define Encode memcpy
#define Decode memcpy
#else

/*
 * Encodes input (u_int32_t) into output (unsigned char). Assumes len is
 * a multiple of 4.
 */

static void
Encode (unsigned char *output, u_int32_t * input, unsigned int len)
{
  unsigned int i;
  u_int32_t *op = (u_int32_t *) output;

  for (i = 0; i < len / 4; i++)
    op[i] = htole32 (input[i]);
}

/*
 * Decodes input (unsigned char) into output (u_int32_t). Assumes len is
 * a multiple of 4.
 */

static void
Decode (u_int32_t * output, const unsigned char *input, unsigned int len)
{
  unsigned int i;
  const u_int32_t *ip = (const u_int32_t *) input;

  for (i = 0; i < len / 4; i++)
    output[i] = le32toh (ip[i]);
}
#endif

static unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* F, G, H and I are basic MD5 functions. */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits. */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/*
 * FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
 * Rotation is separate from addition to prevent recomputation.
 */
#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + (u_int32_t)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + (u_int32_t)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + (u_int32_t)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}
#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + (u_int32_t)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}

/* MD5 initialization. Begins an MD5 operation, writing a new context. */

void
MD5Init (context)
     MD5_CTX *context;
{

  context->count[0] = context->count[1] = 0;

  /* Load magic initialization constants.  */
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;
}

/*
 * MD5 block update operation. Continues an MD5 message-digest
 * operation, processing another message block, and updating the
 * context.
 */

void
MD5Update (context, input, inputLen)
     MD5_CTX *context;
     const unsigned char *input;
     unsigned int inputLen;
{
  unsigned int i, idx, partLen;

  /* Compute number of bytes mod 64 */
  idx = (unsigned int) ((context->count[0] >> 3) & 0x3F);

  /* Update number of bits */
  if ((context->count[0] += ((u_int32_t) inputLen << 3))
      < ((u_int32_t) inputLen << 3))
    context->count[1]++;
  context->count[1] += ((u_int32_t) inputLen >> 29);

  partLen = 64 - idx;

  /* Transform as many times as possible. */
  if (inputLen >= partLen)
    {
      memcpy ((void *) &context->buffer[idx], (const void *) input, partLen);
      MD5Transform (context->state, context->buffer);

      for (i = partLen; i + 63 < inputLen; i += 64)
	MD5Transform (context->state, &input[i]);

      idx = 0;
    }
  else
    i = 0;

  /* Buffer remaining input */
  memcpy ((void *) &context->buffer[idx], (const void *) &input[i],
	  inputLen - i);
}

/*
 * MD5 padding. Adds padding followed by original length.
 */

void
MD5Pad (context)
     MD5_CTX *context;
{
  unsigned char bits[8];
  unsigned int idx, padLen;

  /* Save number of bits */
  Encode (bits, context->count, 8);

  /* Pad out to 56 mod 64. */
  idx = (unsigned int) ((context->count[0] >> 3) & 0x3f);
  padLen = (idx < 56) ? (56 - idx) : (120 - idx);
  MD5Update (context, PADDING, padLen);

  /* Append length (before padding) */
  MD5Update (context, bits, 8);
}

/*
 * MD5 finalization. Ends an MD5 message-digest operation, writing the
 * the message digest and zeroizing the context.
 */

void
MD5Final (digest, context)
     unsigned char digest[16];
     MD5_CTX *context;
{
  /* Do padding. */
  MD5Pad (context);

  /* Store state in digest */
  Encode (digest, context->state, 16);

  /* Zeroize sensitive information. */
  memset ((void *) context, 0, sizeof (*context));
}

/* MD5 basic transformation. Transforms state based on block. */

static void
MD5Transform (state, block)
     u_int32_t state[4];
     const unsigned char block[64];
{
  u_int32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];

  Decode (x, block, 64);

  /* Round 1 */
#define S11 7
#define S12 12
#define S13 17
#define S14 22
  FF (a, b, c, d, x[0], S11, 0xd76aa478);	/* 1 */
  FF (d, a, b, c, x[1], S12, 0xe8c7b756);	/* 2 */
  FF (c, d, a, b, x[2], S13, 0x242070db);	/* 3 */
  FF (b, c, d, a, x[3], S14, 0xc1bdceee);	/* 4 */
  FF (a, b, c, d, x[4], S11, 0xf57c0faf);	/* 5 */
  FF (d, a, b, c, x[5], S12, 0x4787c62a);	/* 6 */
  FF (c, d, a, b, x[6], S13, 0xa8304613);	/* 7 */
  FF (b, c, d, a, x[7], S14, 0xfd469501);	/* 8 */
  FF (a, b, c, d, x[8], S11, 0x698098d8);	/* 9 */
  FF (d, a, b, c, x[9], S12, 0x8b44f7af);	/* 10 */
  FF (c, d, a, b, x[10], S13, 0xffff5bb1);	/* 11 */
  FF (b, c, d, a, x[11], S14, 0x895cd7be);	/* 12 */
  FF (a, b, c, d, x[12], S11, 0x6b901122);	/* 13 */
  FF (d, a, b, c, x[13], S12, 0xfd987193);	/* 14 */
  FF (c, d, a, b, x[14], S13, 0xa679438e);	/* 15 */
  FF (b, c, d, a, x[15], S14, 0x49b40821);	/* 16 */

  /* Round 2 */
#define S21 5
#define S22 9
#define S23 14
#define S24 20
  GG (a, b, c, d, x[1], S21, 0xf61e2562);	/* 17 */
  GG (d, a, b, c, x[6], S22, 0xc040b340);	/* 18 */
  GG (c, d, a, b, x[11], S23, 0x265e5a51);	/* 19 */
  GG (b, c, d, a, x[0], S24, 0xe9b6c7aa);	/* 20 */
  GG (a, b, c, d, x[5], S21, 0xd62f105d);	/* 21 */
  GG (d, a, b, c, x[10], S22, 0x2441453);	/* 22 */
  GG (c, d, a, b, x[15], S23, 0xd8a1e681);	/* 23 */
  GG (b, c, d, a, x[4], S24, 0xe7d3fbc8);	/* 24 */
  GG (a, b, c, d, x[9], S21, 0x21e1cde6);	/* 25 */
  GG (d, a, b, c, x[14], S22, 0xc33707d6);	/* 26 */
  GG (c, d, a, b, x[3], S23, 0xf4d50d87);	/* 27 */
  GG (b, c, d, a, x[8], S24, 0x455a14ed);	/* 28 */
  GG (a, b, c, d, x[13], S21, 0xa9e3e905);	/* 29 */
  GG (d, a, b, c, x[2], S22, 0xfcefa3f8);	/* 30 */
  GG (c, d, a, b, x[7], S23, 0x676f02d9);	/* 31 */
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8a);	/* 32 */

  /* Round 3 */
#define S31 4
#define S32 11
#define S33 16
#define S34 23
  HH (a, b, c, d, x[5], S31, 0xfffa3942);	/* 33 */
  HH (d, a, b, c, x[8], S32, 0x8771f681);	/* 34 */
  HH (c, d, a, b, x[11], S33, 0x6d9d6122);	/* 35 */
  HH (b, c, d, a, x[14], S34, 0xfde5380c);	/* 36 */
  HH (a, b, c, d, x[1], S31, 0xa4beea44);	/* 37 */
  HH (d, a, b, c, x[4], S32, 0x4bdecfa9);	/* 38 */
  HH (c, d, a, b, x[7], S33, 0xf6bb4b60);	/* 39 */
  HH (b, c, d, a, x[10], S34, 0xbebfbc70);	/* 40 */
  HH (a, b, c, d, x[13], S31, 0x289b7ec6);	/* 41 */
  HH (d, a, b, c, x[0], S32, 0xeaa127fa);	/* 42 */
  HH (c, d, a, b, x[3], S33, 0xd4ef3085);	/* 43 */
  HH (b, c, d, a, x[6], S34, 0x4881d05);	/* 44 */
  HH (a, b, c, d, x[9], S31, 0xd9d4d039);	/* 45 */
  HH (d, a, b, c, x[12], S32, 0xe6db99e5);	/* 46 */
  HH (c, d, a, b, x[15], S33, 0x1fa27cf8);	/* 47 */
  HH (b, c, d, a, x[2], S34, 0xc4ac5665);	/* 48 */

  /* Round 4 */
#define S41 6
#define S42 10
#define S43 15
#define S44 21
  II (a, b, c, d, x[0], S41, 0xf4292244);	/* 49 */
  II (d, a, b, c, x[7], S42, 0x432aff97);	/* 50 */
  II (c, d, a, b, x[14], S43, 0xab9423a7);	/* 51 */
  II (b, c, d, a, x[5], S44, 0xfc93a039);	/* 52 */
  II (a, b, c, d, x[12], S41, 0x655b59c3);	/* 53 */
  II (d, a, b, c, x[3], S42, 0x8f0ccc92);	/* 54 */
  II (c, d, a, b, x[10], S43, 0xffeff47d);	/* 55 */
  II (b, c, d, a, x[1], S44, 0x85845dd1);	/* 56 */
  II (a, b, c, d, x[8], S41, 0x6fa87e4f);	/* 57 */
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0);	/* 58 */
  II (c, d, a, b, x[6], S43, 0xa3014314);	/* 59 */
  II (b, c, d, a, x[13], S44, 0x4e0811a1);	/* 60 */
  II (a, b, c, d, x[4], S41, 0xf7537e82);	/* 61 */
  II (d, a, b, c, x[11], S42, 0xbd3af235);	/* 62 */
  II (c, d, a, b, x[2], S43, 0x2ad7d2bb);	/* 63 */
  II (b, c, d, a, x[9], S44, 0xeb86d391);	/* 64 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  /* Zeroize sensitive information. */
  memset ((void *) x, 0, sizeof (x));
}

// a partir d'aquí, afegit de /usr/src/lib/libmd/mdXhl.c

char *
MD5End (MD5_CTX * ctx, char *buf)
{
  int i;
  unsigned char digest[LENGTH];
  static const char hex[] = "0123456789abcdef";

  if (!buf)
    buf = malloc (2 * LENGTH + 1);
  if (!buf)
    return 0;
  MD5Final (digest, ctx);
  for (i = 0; i < LENGTH; i++)
    {
      buf[i + i] = hex[digest[i] >> 4];
      buf[i + i + 1] = hex[digest[i] & 0x0f];
    }
  buf[i + i] = '\0';
  return buf;
}

char *
MD5File (const char *filename, char *buf)
{
  return (MD5FileChunk (filename, buf, 0, 0));
}

char *
MD5FileChunk (const char *filename, char *buf, off_t ofs, off_t len)
{
  unsigned char buffer[BUFSIZ];
  MD5_CTX ctx;
  struct stat stbuf;
  int f, i, e;
  off_t n;

  MD5Init (&ctx);
  f = open (filename, O_RDONLY);
  if (f < 0)
    return 0;
  if (fstat (f, &stbuf) < 0)
    return 0;
  if (ofs > stbuf.st_size)
    ofs = stbuf.st_size;
  if ((len == 0) || (len > stbuf.st_size - ofs))
    len = stbuf.st_size - ofs;
  if (lseek (f, ofs, SEEK_SET) < 0)
    return 0;
  n = len;
  i = 0;
  while (n > 0)
    {
      if ((unsigned) n > sizeof (buffer))	// <== corregit per a evitar un warning
	i = read (f, buffer, sizeof (buffer));
      else
	i = read (f, buffer, n);
      if (i < 0)
	break;
      MD5Update (&ctx, buffer, i);
      n -= i;
    }
  e = errno;
  close (f);
  errno = e;
  if (i < 0)
    return 0;
  return (MD5End (&ctx, buf));
}

char *
MD5Data (const void *data, unsigned int len, char *buf)
{
  MD5_CTX ctx;

  MD5Init (&ctx);
  MD5Update (&ctx, data, len);
  return (MD5End (&ctx, buf));
}

/**
 * @Authors:
 * Albert Hilazo
 * Daniel Madurell
 */

void stringToMd5 (const char* string , char md5string[MD5_BLOCK_LENGTH]) {

  int i;
  unsigned char digest[16];
  struct MD5Context context;

  MD5Init(&context);
  MD5Update(&context, string, strlen(string));
  MD5Final(digest, &context);
  //printf("%s\n", digest);

  for(i = 0; i < 16; ++i) {
    sprintf(&md5string[i*2], "%02x", (unsigned int)digest[i]);
  }
}
