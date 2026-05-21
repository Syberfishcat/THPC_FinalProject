#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

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
    std::cout << "Read " << N << "cities:" << std::endl;
    for (int i = 0; i < N; ++i) {
        std::cout << "City " << i << ": (" << x[i] << ", " << y[i] << ")" << std::endl;
    }

    std::vector<int> route(N);
    for (int i = 0; i < N; ++i) {
        route[i] = i;
    }

    double length = route_length(route, x, y);
    std::cout << "Route Length: " << length << std::endl;

    return 0;
}