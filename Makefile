.PHONY: run

run: tlist
	./tlist

tlist: tlist.cpp
	g++ -Wall -Wextra -Werror -std=gnu++11 -o tlist tlist.cpp
