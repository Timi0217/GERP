CXX = clang++
CXXFLAGS = -g3 -Wall -Wextra -std=c++11
LDFLAGS = -g3

gerp: main.o database.o FSTree.o DirNode.o
	${CXX} ${LDFLAGS} ${CXXFLAGS} database.o main.o FSTree.o \
	DirNode.o -o gerp

database.o: database.h database.cpp FSTree.h DirNode.h
	${CXX} ${LDFLAGS} ${CXXFLAGS} -c database.cpp 
	
main.o: database.h main.cpp
	${CXX} ${LDFLAGS} ${CXXFLAGS} -c main.cpp database.cpp
	
clean:
	rm main.o database.o
	
provide: 
	provide comp15 proj2part3 main.cpp database.h database.cpp FSTree.h \
	DirNode.h Makefile README testDataBase.cpp
