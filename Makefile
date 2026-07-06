CXX = g++
CXXFLAGS = -Wall -g

main.exe: main.o
	$(CXX) main.o -o main.exe

main.o: main.cpp graph.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

.PHONY: clean
clean:
	rm *.o *.exe

.PHONY: doc
doc:
	doxygen