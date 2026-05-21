#include <iostream>
#include <fstream>
#include <vector>

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

    return 0;
}