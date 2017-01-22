#include <string.h>
#include "siphash.h"

#define ROTATE_LEFT(x, b) (unsigned long)(((x) << (b)) | ((x) >> (64 - (b))))

#define COMPRESS                \
  v0 += v1;                     \
  v2 += v3;                     \
  v1 = ROTATE_LEFT(v1, 13);     \
  v3 = ROTATE_LEFT(v3, 16);     \
  v1 ^= v0;                     \
  v3 ^= v2;                     \
  v0 = ROTATE_LEFT(v0, 32);     \
  v2 += v1;                     \
  v0 += v3;                     \
  v1 = ROTATE_LEFT(v1, 17);     \
  v3 = ROTATE_LEFT(v3, 21);     \
  v1 ^= v2;                     \
  v3 ^= v0;                     \
  v2 = ROTATE_LEFT(v2, 32);

#define DIGEST_BLOCK            \
  v3 ^= m;                      \
  do {                          \
    int i;                      \
    for(i = 0; i < c; i++){     \
      COMPRESS                  \
    }                           \
  } while (0);                  \
  v0 ^= m;

#define U8TO64_LE(p)                                        		\
  (((uint64_t)((p)[0])) | ((uint64_t)((p)[1]) << 8) |     			\
   ((uint64_t)((p)[2]) << 16) | ((uint64_t)((p)[3]) << 24) | 		\
   ((uint64_t)((p)[4]) << 32) | ((uint64_t)((p)[5]) << 40) | 		\
   ((uint64_t)((p)[6]) << 48) | ((uint64_t)((p)[7]) << 56))

SipHash::SipHash(char key[16], int c, int d) {
		this->c = c;
		this->d = d;

		uint64_t k0 = U8TO64_LE(key);
		uint64_t k1 = U8TO64_LE(key + 8);

		this->v0 = (0x736f6d6570736575 ^ k0);
		this->v1 = (0x646f72616e646f6d ^ k1);
		this->v2 = (0x6c7967656e657261 ^ k0);
		this->v3 = (0x7465646279746573 ^ k1);

		this->m_idx = 0;
		this->input_len = 0;
		this->m = 0;
}

SipHash::~SipHash() {
		// TODO Auto-generated destructor stub
}

SipHash* SipHash::update(char b) {
		input_len++;
		m |= (((long) b & 0xff) << (m_idx++ * 8));
		if (m_idx >= 8) {
				DIGEST_BLOCK
				m_idx = 0;
				m = 0;
		}
		return this;
}

SipHash* SipHash::update(char data[]) {
		const char *pszChar = data;
		while (pszChar != NULL && *pszChar != '\0') {
				update(*pszChar);
				pszChar++;
		}
		return this;
}

uint64_t SipHash::digest() {
		while (m_idx < 7) {
				m |= 0 << (m_idx++ * 8);
		}

		m |= ((uint64_t) input_len) << (m_idx * 8);

		DIGEST_BLOCK

		v2 ^= 0xff;

		for(int i = 0; i < d; i++){
				COMPRESS
		}

		return ((uint64_t) v0 ^ v1 ^ v2 ^ v3);
}
