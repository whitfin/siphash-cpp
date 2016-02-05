# SipHash

[![Build Status](https://travis-ci.org/zackehh/siphash-cpp.svg?branch=master)](https://travis-ci.org/zackehh/siphash-cpp)

A C++ implementation of the SipHash cryptographic hash family. Supports any variation, although defaults to the widely used SipHash-2-4.

This library was heavily influenced by [veorq's C implementation](https://github.com/veorq/siphash), but modified to be made a little more user-friendly (IMO). The intention is to pick this over into a Node lib for faster execution.

## Usage

You can use this library as either an extension of your own project, or via the command line using a wrapper.

### Command Line Setup

You have to build manually if you want to create the command line interface:

```bash
$ cd /tmp
$ git clone https://github.com/zackehh/siphash-cpp.git
$ cd siphash-cpp
$ make install
```

From there you can access `siphash` through your terminal, as follows:

```bash
$ siphash -k "0123456789ABCDEF" -i "hello" -c 2 -d 4
```

The applicable options exist of:

- `-k` - the key to use for the hash (required)
- `-i` - the input string to hash    (required)
- `-f` - a file path to hash
- `-c` - the rounds of C compression (defaults to c)
- `-d` - the rounds of D compression (defaults to 4)

Note:

- `-k` must be provided, along with either of `-i` or `-f`.
- Be careful when using `-f`, as the file input is buffered into memory.
- Avoid adventurous use cases, this is a tool designed for debugging rather than heavy use.

## Testing

The tests are very barebones at the moment, but you can run them as follows:

```c
$ make siphash_test
$ ./build/test/siphash_test
```