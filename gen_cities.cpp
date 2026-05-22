#include <iostream>
#include <fstream>
#include <random>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <num_cities> <output_file> [seed]\n";
        return 1;
    }
    int N = std::stoi(argv[1]);
    std::string out_file = argv[2];
    unsigned seed = (argc >= 4) ? std::stoul(argv[3]) : 12345;

    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::ofstream fout(out_file);
    if (!fout) {
        std::cerr << "Cannot open " << out_file << "\n";
        return 1;
    }

    fout << N << "\n";
    for (int i = 0; i < N; ++i) {
        fout << dist(rng) << " " << dist(rng) << "\n";
    }

    std::cerr << "Generated " << N << " cities in " << out_file << "\n";
    return 0;
}