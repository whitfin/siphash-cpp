B = build
BT = $(B)/test
CFLAGS = -g -O3 -ansi -pedantic -Wall -Wextra -std=c++03
PREFIX ?= /usr/local
SRC = main.cpp src/siphash.cpp
TEST = test/test.cpp src/siphash.cpp

all:            siphash
siphash:        $(SRC)
				mkdir -p $(B)
				$(CXX) $(CFLAGS) $^ -o $(B)/$@
clean:
				$(RM) -r $(B)
install:        siphash
				cp -f $(B)/siphash $(PREFIX)/bin/siphash
siphash_test:   $(TEST)
				mkdir -p $(BT)
				$(CXX) $^ $(CFLAGS) -o $(BT)/siphash_test