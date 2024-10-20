CPP = g++ src/main.cpp
ARGS = -std=c++20 -g -Wall -pedantic -Wextra

DEPS_RAYLIB  = -L C:/raylib/lib -lraylib -lgdi32 -lwinmm
DEPS_SQLITE3 = -L C:/sqlite3/lib -lsqlite3

all: compile finish open

compile:
	$(CPP) $(ARGS) -I C:/raylib/include -I C:/sqlite3/include -o main.exe $(DEPS_RAYLIB) $(DEPS_SQLITE3)

finish:
	@echo -e "\033[0;32m\nDone at $(shell date +%T)\n\e[0m"

open:
	@echo -e "\033[0;94mRunning exe...\e[0m"
	@main.exe
