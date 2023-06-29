main:
	g++ src/k_combination.cpp src/prepare_input.cpp src/bhk.cpp src/main.cpp -o main -Wall -finline-functions -flto
	./main