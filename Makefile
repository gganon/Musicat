# Specify compile
CC = g++

# Specify compiler flag
CCF = -std=c++17 -Wall -Wextra -g

# Specify include folder
INC = -I src/include -I/usr/include/opus # -lxml2 -lz -llzma -licui18n -licuuc -licudata -lm

# Specify libs to use
LIB = -ldpp -pthread -lcurl -lcurlpp -logg -lopus -lopusfile # -lllhttp

# Specify source file
SRC = src/main.cpp libs/opusfile/src/*.c # src/include/*.cpp

# Specify out file
OUT = exe/Shasha

all: $(SRC)
	$(CC) $(CCF) $(INC) $(LIB) $(SRC) -o $(OUT)

ex:
	g++ -Wall -Wextra -g -Isrc/include exec.cpp -o ex

ex2:
	g++ -Wall -Wextra -g -Isrc/include exec2.cpp -o ex2
