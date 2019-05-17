CC=g++ -std=c++11 -Wno-deprecated

all: logger.out

clean:
	rm logger.o main.o logger.out

logger.out: main.o logger.o
	$(CC) -o $@ $^

main.o: main.cpp
	$(CC) -c $^

logger.o: logger.cpp logger.h
	$(CC) -c $^
