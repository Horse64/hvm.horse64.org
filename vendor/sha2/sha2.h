
// File written/added to fix build for HVM,
// consider this file public domain.

unsigned char *SHA512(void *data, unsigned int data_len, unsigned char *digest);

#define SHA512_BLOCK_LENGTH     128
#define SHA512_DIGEST_LENGTH        64
#define SHA512_DIGEST_STRING_LENGTH (SHA512_DIGEST_LENGTH * 2 + 1)

