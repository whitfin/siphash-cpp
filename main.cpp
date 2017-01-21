#include <getopt.h>
#include <iostream>
#include <inttypes.h>
#include <fstream>
#include <vector>
#include <sys/stat.h>

#include "src/siphash.h"

using namespace std;

int main(int argc, char *argv[]) {
		char* key = NULL;
		char* input = NULL;
		char* fname = NULL;

		int c = 0, d = 0, option = 0;

		if (argc < 2) {
				printf("Usage: siphash -k <key> [-i <input> | -f <filepath>] [-c=2] [-d=4]\n");
				return 1;
		}

		while ((option = getopt(argc, argv, "f:k:i:c:d:")) != -1) {
				switch (option) {
						case 'c':
								c = atoi(optarg);
								break;
						case 'd':
								d = atoi(optarg);
								break;
						case 'f':
								if (access(optarg, F_OK) == -1) {
										break;
								}
								fname = optarg;
								break;
						case 'i':
								input = optarg;
								break;
						case 'k':
								key = optarg;
								break;
				}
		}

		if (key == NULL || (input == NULL && fname == NULL)) {
				printf("Usage: siphash -k <key> [-i <input> | -f <filepath>] [-c=2] [-d=4]\n");
				return 1;
		}

		if (strlen(key) != 16) {
				printf("Key must consist of exactly 16 bytes!\n");
				return 1;
		}

		if (c <= 0) {
				c = 2;
		}

		if (d <= 0) {
				d = 4;
		}

		SipHash sipHash(key, c, d);

		if (fname != NULL) {
				struct stat filestatus;
				stat(fname, &filestatus);

				size_t total_size = filestatus.st_size;
				size_t chunk_size = 16;

				size_t total_chunks = total_size / chunk_size;
				size_t last_chunk_size = total_size % chunk_size;

				if (last_chunk_size != 0) {
						++total_chunks;
				} else {
						last_chunk_size = chunk_size;
				}

				ifstream file(fname, ifstream::binary);

				for (size_t chunk = 0; chunk < total_chunks; ++chunk) {
						size_t this_chunk_size = chunk == total_chunks - 1
										? last_chunk_size
										: chunk_size;

						vector<char> chunk_data(this_chunk_size);
						file.read(&chunk_data[0], this_chunk_size);

						for (size_t i = 0; i < this_chunk_size; i++) {
								sipHash.update(chunk_data[i]);
						}
				}
		}
		else {
				sipHash.update(input);
		}

		printf("%" PRIu64 "\n", sipHash.finish());

		return 0;
}
