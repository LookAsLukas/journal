.PHONY: all clean

all: libjournal.so journal_writer

journal.o: lib/Journal.cpp
	g++ -std=c++17 -c -o $@ $^ -fPIC

message.o: lib/Message.cpp
	g++ -std=c++17 -c -o $@ $^ -fPIC

libjournal.so: journal.o message.o
	g++ -std=c++17 -shared -o $@ $^

journal_writer: src/main.cpp
	g++ -std=c++17 -I./lib -o $@ $< -L. -ljournal -Wl,-rpath=.

clean:
	rm journal.o message.o libjournal.so journal_writer
