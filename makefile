main:
	g++ src/prepare_input.cpp src/bhk.cpp src/main.cpp -o main -Wall -finline-functions -flto -Ofast -march=native