#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <getopt.h>
#include <inttypes.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include "src/siphash.h"

using namespace std;

int show_help() {
    printf("Usage: siphash -k <key> [-i <input> | -f <filepath>] [-b=10] [-c=2] [-d=4]\n");
		return 1;
}

int main(int argc, char *argv[]) {
    int dc = fileno(stdin);

#ifdef _WIN32
    bool tty = _isatty(dc);
#else    
    bool tty = isatty(dc);
#endif

    char* key = NULL;
    char* input = NULL;
    char* fname = NULL;

    int b = 10, c = 0, d = 0, option = 0;

    if (argc < 2) {
        return show_help();
    }

    while ((option = getopt(argc, argv, "f:k:i:c:d:b:")) != -1) {
        switch (option) {
            case 'b':
                b = atoi(optarg);
                break;
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

    if (key == NULL || (tty && input == NULL && fname == NULL)) {
        return show_help();
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

    SipHash sip(key, c, d);

    if (!tty) {
        char *chunk = new char[8192];

        while (!cin.eof()) {
            cin.read(chunk, 8192);

            for(int i = 0; i < cin.gcount(); i++){
                sip.update(chunk[i]);
            }
        }
    }
    else if (fname != NULL) {
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
                sip.update(chunk_data[i]);
            }
        }
    }
    else {
        sip.update(input);
    }

    cout << setbase(b);
    cout << sip.digest() << endl;

    return 0;
}
