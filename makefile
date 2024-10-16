CPP = g++ src/main.cpp
ARGS = -std=c++20 -g -Wall -pedantic -Wextra

DEPS_RAYLIB = -L C:/raylib/lib -lraylib -lgdi32 -lwinmm

all: compile finish open

compile:
	$(CPP) $(ARGS) -I C:/raylib/include -I C:/sqlite3 -o main.exe $(DEPS_RAYLIB)

finish:
	@echo -e "\033[0;32m\nDone at $(shell date +%T)\n\e[0m"

open:
	@echo -e "\033[0;94mRunning exe...\e[0m"
	@main.exe
