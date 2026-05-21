CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

tsp: tsp.cpp
	$(CXX) $(CXXFLAGS) -o tsp tsp.cpp

clean:
	rm -f tsp