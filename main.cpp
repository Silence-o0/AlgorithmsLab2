#include <iostream>
#include <string>
#include <cmath>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

using namespace std;

size_t mod_pow(size_t a, size_t n, size_t m) {
    size_t result = 1;
    a %= m;
    while (n) {
        if (n % 2 == 1)
            result = (result * a) % m;

        a = (a * a) % m;
        n /= 2;
    }
    return result;
}

class CountingBloomFilter {
private:
    size_t m;
    size_t l;
    boost::dynamic_bitset<> filter;
    static constexpr size_t prime = 29;

    size_t hash_string(const string &s) {
        size_t sum = 0;
        for (int i = 0; i < s.size(); i++) {
            sum = (sum + (s[i] - 'a') * mod_pow(prime, i, m) % m) % m;
        }
        return sum;
    }

public:
    CountingBloomFilter(size_t n, double p) {
        m = ceil(-(n * log(p)) / (log(2) * log(2)));
        l = ceil(-(log(p) / log(2)));
        filter.resize(m, 0);
    }

    void add(const std::string &item) {
        for (size_t i = 0; i < l; i++) {
            size_t hash_value = hash_string(item + char(('a' + i) % 26));
            filter.set(hash_value);
        }
    }

    bool contains(const std::string &item) {
        for (size_t i = 0; i < l; i++) {
            size_t hash_value = hash_string(item + char(('a' + i) % 26));
            if (filter.test(hash_value) == 0) return false; // це не пітон. треба дужечки різні. ахахахахахахах
        }
        return true;
    }
};

int main() {
    size_t n = 1e6;
    double p = 0.01;

    CountingBloomFilter bloom_filter(n, p);
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        char operation = line[0];
        std::string item = line.substr(2);

        if (operation == '+') {
            bloom_filter.add(item);
        } else if (operation == '?') {
            if (bloom_filter.contains(item)) {
                std::cout << "Y" << std::endl;
            } else {
                std::cout << "N" << std::endl;
            }
        } else if (operation == '#') {
            return 0;
        } else {
            std::cerr << "Невідома операція: " << operation << std::endl;
        }
    }
    return 0;
}
