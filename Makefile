all: wwivcat userlist wwivdump

wwivcat: wwivcat.cpp
	g++ -std=c++11 -o wwivcat wwivcat.cpp

userlist: userlist.cpp
	g++ -o userlist userlist.cpp -std=c++11

wwivdump: wwivdump.cpp
	g++ -o wwivdump  wwivdump.cpp -ggdb -D_DEBUG -std=c++11 -lmenu -lncurses -fpermissive
