#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

double route_length(const std::vector<int>& route,
                    const std::vector<double>& x,
                    const std::vector<double>& y) {
    double length = 0.0;
    int N = route.size();
    for (int i = 0; i < N; ++i) {
        int j = (i + 1) % N; // Next city index
        double dx = x[route[i]] - x[route[j]];
        double dy = y[route[i]] - y[route[j]];
        length += std::sqrt(dx * dx + dy * dy);
    }
    return length;
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    std::ifstream fin(argv[1]);
    if(!fin) {
        std::cerr << "Cannot open file: " << argv[1] << std::endl;
        return 1;
    }
    int N;
    fin >> N;
    std::vector<double> x(N), y(N);
    for (int i = 0; i < N; ++i) {
        fin >> x[i] >> y[i];
    }

    int num_trials = 10000;
    if (argc >= 3) {
        num_trials = std::stoi(argv[2]);
    }
    
    // Random seach
    std::mt19937 rng(42);
    std::vector<int> route(N);
 
    for (int i = 0; i < N; ++i) route[i] = i; // Initial
    double best_length = route_length(route, x, y);
    std::vector<int> best_route = route;

    for (int trial = 0; trial < num_trials; ++trial) {
        std::shuffle(route.begin(), route.end(), rng);
        double length = route_length(route, x, y);
        if (length < best_length) {
            best_length = length;
            best_route = route;
        }
    }
    std::cout << "Best Length: " << best_length << std::endl;
    std::cout << "Best Route:";
    for (const auto& c : best_route) std::cout << " " << c;
    return 0;
}