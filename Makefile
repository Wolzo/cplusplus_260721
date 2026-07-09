CXX = g++
CXXFLAGS = -Wall -std=c++11

main.exe: main.o
	$(CXX) main.o -o main.exe

main.o: main.cpp custom_types.hpp graph.hpp edge_already_exists.hpp edge_not_found.hpp node_already_exists.hpp node_not_found.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

.PHONY: clean
clean:
	rm *.o *.exe

.PHONY: doc
doc:
	doxygen