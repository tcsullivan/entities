all:
	g++ -std=c++17 -ggdb -pedantic -Wall -Wextra main.cpp -o main -lentityx

test:
	g++ -std=c++17 -Wall -Wextra bench.cpp -o test -O1
	
