PROG=main.exe
CC=g++
CFLAGS=-g -Wall -Werror -std=c++20
OBJ=main.o md5.o

$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	del $(OBJ) $(PROG)
