B = build
BT = $(B)/test
CFLAGS = -g -O3 -ansi -pedantic -Wall -Wextra -std=c++03
PREFIX ?= /usr/local
SRC = main.cpp src/siphash.cpp
TARGET ?= $(PREFIX)/bin
TEST = test/test.cpp src/siphash.cpp

all:            siphash
siphash:        $(SRC)
				mkdir -p $(B)
				$(CXX) $(CFLAGS) $^ -o $(B)/$@
clean:
				$(RM) -r $(B)
install:        siphash
				mkdir -p $(TARGET)
				cp -f $(B)/siphash $(TARGET)/siphash
siphash_test:   $(TEST)
				mkdir -p $(BT)
				$(CXX) $^ $(CFLAGS) -o $(BT)/siphash_test
