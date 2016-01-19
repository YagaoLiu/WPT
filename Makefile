MF=     Makefile
 
CC=     g++
 
CFLAGS= -g -fopenmp -msse4.2 -fomit-frame-pointer -funroll-loops 
 
LFLAGS= -std=c++11 -O3 -DNDEBUG -I ./libsdsl/include/ -L ./libsdsl/lib/ -lsdsl -ldivsufsort -ldivsufsort64 -Wl,-rpath=$(PWD)/libsdsl/lib

EXE=    wpt 
 
SRC=    main.cpp input.cpp matching.cpp preparation.cpp parray.cpp wptable.cpp drittel.cpp lcve.cpp operate.cpp
 
HD=     global.h defs.h util.h    Makefile
 
# 
# No need to edit below this line 
# 
 
.SUFFIXES: 
.SUFFIXES: .cpp .o 
 
OBJ=    $(SRC:.cpp=.o) 
 
.cpp.o: 
	$(CC) $(CFLAGS)-c $(LFLAGS) $< 
 
all:    $(EXE) 
 
$(EXE): $(OBJ) 
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS) 
 
$(OBJ): $(MF) $(HD) 
 
clean: 
	rm -f $(OBJ) $(EXE) *~
	
clean-all:
	rm -f $(OBJ) $(EXE) *~
	rm -r libsdsl
	rm -r sdsl-lite

