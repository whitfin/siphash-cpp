#ifndef SIPHASH_NODE_SIPHASH_H
#define SIPHASH_NODE_SIPHASH_H

#include <inttypes.h>

class SipHash {
private:
		int c, d, m_idx;
		uint64_t v0, v1, v2, v3, m;
		unsigned char input_len;
public:
		SipHash(char key[16], int c = 2, int d = 4);
		~SipHash();
		SipHash* update(char data);
		SipHash* update(char data[]);
	  uint64_t digest();
};

#endif
