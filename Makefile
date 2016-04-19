main: main.cpp
	g++ -o cpp.out -O3 -ansi -Wall main.cpp -lm

cpp11: main.cpp
	g++ -o 11.out -g -O3 -ansi -Wall -std=c++11 main.cpp -lm

clean:
	rm -rf *.out
