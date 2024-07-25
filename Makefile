CXX=g++
CXXFLAGS=-Wall -g
TARGET=encryptor
LDFLAGS=-lncurses  

all: $(TARGET)

$(TARGET): main.o cipher.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.cpp cipher.h
	$(CXX) $(CXXFLAGS) -c main.cpp

cipher.o: cipher.cpp cipher.h
	$(CXX) $(CXXFLAGS) -c cipher.cpp

clean:
	rm -f *.o $(TARGET)

.PHONY: clean

