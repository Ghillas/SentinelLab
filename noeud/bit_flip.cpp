#include <iostream>
#include <random>

int main(int argc, char* argv[]) {
    double proba = (argc > 1) ? std::stod(argv[1]) : 0.001;
    std::mt19937 rng(42);
    std::bernoulli_distribution dist(proba);

    char c;
    while (std::cin.get(c)) {
        unsigned char b = static_cast<unsigned char>(c);
        if (dist(rng)) {
            b ^= (1 << (rng() % 8));
        }
        std::cout.put(static_cast<char>(b));
    }
    std::cout.flush();
    return 0;
}
