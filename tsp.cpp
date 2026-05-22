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

void baseline(std::vector<double>& x, std::vector<double>& y, int num_trials) {
    std::mt19937 rng(42);
    std::vector<int> route(x.size());
    int N = route.size();
    for (int i = 0; i < N; ++i) route[i] = i; // Initial
    std::shuffle(route.begin(), route.end(), rng);
    double best_length = route_length(route, x, y);
    std::vector<int> best_route = route;

    // Local search: swap two cities and check if it improves the route
    for (int trial = 0; trial < num_trials; ++trial) {
        std::vector<int> candidate = best_route;
        
        std::uniform_int_distribution<int> position(0, N - 1);
        int i = position(rng);
        int j = position(rng);
        while (j == i) {
            j = position(rng);
        }
        std::swap(candidate[i], candidate[j]);
        double length = route_length(candidate, x, y);
        if (length < best_length) {
            best_length = length;
            best_route = candidate;
            std::cerr << "  trial " << trial << ": new best = " << best_length << std::endl;
        }
    }

    std::cout << "Best Length: " << best_length << std::endl;
    std::cout << "Best Route:";
    for (const auto& c : best_route) std::cout << " " << c;
}

void GA(std::vector<double>& x, std::vector<double>& y) {
    int pop_size = 100;
    //int num_generations = 1000;
    int N = x.size();
    std::mt19937 rng(42);

    // Initialize population
    std::vector<std::vector<int>> population(pop_size, std::vector<int>(N));
    for(int i = 0; i < pop_size; ++i) {
        for(int j = 0; j < N; ++j) population[i][j] = j;
        std::shuffle(population[i].begin(), population[i].end(), rng);
    }

    // Evaluate fitness
    std::vector<double> fitness(pop_size);
    for(int i = 0; i < pop_size; ++i) {
        fitness[i] = route_length(population[i], x, y);
    }

    //Find best solution
    double best_length = fitness[0];
    std::vector<int> best_route = population[0];
    for(int i = 1; i < pop_size; ++i) {
        if(fitness[i] < best_length) {
            best_length = fitness[i];
            best_route = population[i];
        }
    }
    std::cout << "Best Length: " << best_length << std::endl;
    std::cout << "Best Route:";
    for (const auto& c : best_route) std::cout << " " << c;
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

    // int num_trials = 10000;
    // if (argc >= 3) {
    //     num_trials = std::stoi(argv[2]);
    // }

    //baseline(x, y, num_trials);
    GA(x, y);
    return 0;
}