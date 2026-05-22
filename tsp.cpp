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

int find_best(const std::vector<double>& fitness) {
    int best_index = 0;
    for (int i = 1; i < fitness.size(); ++i) {
        if (fitness[i] < fitness[best_index]) {
            best_index = i;
        }
    }
    return best_index;
}

int next_city_in_parent(const std::vector<int>& parent, int city) {
    int N = parent.size();
    for (int i = 0; i < N; ++i) {
        if (parent[i] == city) {
            return parent[(i + 1) % N];
        }
    }
    return -1; // Should never happen
}

std::vector<int> crossover(const std::vector<int>& p1,
                           const std::vector<int>& p2,
                           const std::vector<double>& x,
                           const std::vector<double>& y,
                           std::mt19937& rng) {
    int N = p1.size();
    std::vector<int> child(N);
    std::vector<bool> used(N, false);
    std::uniform_int_distribution<int> init(0, 1);

    child[0] = init(rng) ? p1[0] : p2[0];
    used[child[0]] = true;

    for(int i = 1; i < N; ++i) {
        int cur = child[i - 1];
        int c1 = next_city_in_parent(p1, cur);
        int c2 = next_city_in_parent(p2, cur);
        if (!used[c1] && !used[c2]) {
            double d1 = std::sqrt(std::pow(x[cur] - x[c1], 2) + std::pow(y[cur] - y[c1], 2));
            double d2 = std::sqrt(std::pow(x[cur] - x[c2], 2) + std::pow(y[cur] - y[c2], 2));
            child[i] = (d1 < d2) ? c1 : c2;
        }else if (!used[c1]) {
            child[i] = c1;
        } else if (!used[c2]) {
            child[i] = c2;
        } else {
            // Both cities are used, pick a random unused city
            std::vector<int> candidates;
            for (int j = 0; j < N; ++j) {
                if (!used[j]) candidates.push_back(j);
            }
            std::uniform_int_distribution<int> candidate_dist(0, candidates.size() - 1);
            child[i] = candidates[candidate_dist(rng)];
        }
        used[child[i]] = true;
    }

    return child;
}

void GA(std::vector<double>& x, std::vector<double>& y) {
    int pop_size = 100;
    int num_generations = 1000;
    int elite_count = 10;
    int N = x.size();

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> position(0, N - 1);

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

    std::cerr << "Initial best: " << fitness[find_best(fitness)] << std::endl;

    // Main GA loop
    for(int gen = 0; gen < num_generations; ++gen) {
        // Selection (elitism)
        std::vector<int> indices(pop_size);
        for(int i = 0; i < pop_size; ++i) indices[i] = i;
        std::partial_sort(indices.begin(), indices.begin() + elite_count, indices.end(),
                          [&](int a, int b) { return fitness[a] < fitness[b]; });
        
        // new population
        std::vector<std::vector<int>> new_population(pop_size, std::vector<int>(N));
        std::uniform_int_distribution<int> elite_pick(0, elite_count - 1);
        std::uniform_real_distribution<double> prob(0.0, 1.0);
        double mutation_rate = 0.1;

        for(int i = 0; i < elite_count; ++i) {
            new_population[i] = population[indices[i]];
        }
        for(int i = elite_count; i < pop_size; ++i) {
            int pa = elite_pick(rng);
            int pb = elite_pick(rng);
            while (pb == pa) pb = elite_pick(rng);

            const std::vector<int>& parent1 = population[indices[pa]];
            const std::vector<int>& parent2 = population[indices[pb]];

            new_population[i] = crossover(parent1, parent2, x, y, rng);

            if(prob(rng) < mutation_rate) {
                int a = position(rng);
                int b = position(rng);
                while (b == a) {
                    b = position(rng);
                }
                std::swap(new_population[i][a], new_population[i][b]);
            }
        }

        // Evaluate new population and replace
        population = new_population;
        for(int i = 0; i < pop_size; ++i) {
            fitness[i] = route_length(population[i], x, y);
        }

        // print progress
        if(gen % 100 == 0 || gen == num_generations - 1) {
            std::cerr << "Generation " << gen << ": best = " << fitness[find_best(fitness)] << std::endl;
        }
    }

    // Output best solution
    int best_index = find_best(fitness);
    
    std::cout << "Best Length: " << fitness[best_index] << std::endl;
    std::cout << "Best Route:";
    for (const auto& c : population[best_index]) std::cout << " " << c;
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

    //baseline(x, y, 1000000);
    GA(x, y);
    return 0;
}
