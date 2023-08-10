main:
	g++ src/prepare_input.cpp src/bhk.cpp src/btk.cpp src/main.cpp -o main -Wall -finline-functions -flto -O3 -march=native