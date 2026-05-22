CXX = mpicxx
CXXFLAGS = -std=c++17 -O2 -Wall

all: tsp gen_cities

tsp: tsp.cpp
	$(CXX) $(CXXFLAGS) -o tsp tsp.cpp

gen_cities: gen_cities.cpp
	$(CXX) $(CXXFLAGS) -o gen_cities gen_cities.cpp

clean:
	rm -f tsp gen_cities