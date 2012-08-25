a.out: $(wildcard *.cpp *.hpp)
	g++ -Wall -Werror -std=c++11 *.cpp

run: a.out
	./a.out | dzen2 -ta l -h 17 -fn -inconsolata-terminal-bold-*-*--13-*-*-*-*-*-*-*
