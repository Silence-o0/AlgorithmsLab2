#include <iostream>
#include <string>
#include <cmath>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <random>

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

random_device rd;
std::mt19937 gen(rd());

string generate_random_string(int length) {
    string result;
    string letters = "abcdefghijklmnopqrstuvwxyz";
    uniform_int_distribution<> dist(0, int(letters.size()) - 1);

    for (int i = 0; i < length; ++i) {
        result += letters[dist(gen)];
    }
    return result;
}

vector<string> generate_random_strings(int count) {
    int max_length = 15;
    vector<std::string> strings;
    for (int i = 0; i < count; ++i) {
        uniform_int_distribution<> str_length(1, max_length);
        int length = str_length(gen);
        strings.push_back(generate_random_string(length));
    }
    return strings;
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

    void add(const string &item) {
        for (size_t i = 0; i < l; i++) {
            size_t hash_value = hash_string(item + char(('a' + i) % 26));
            filter.set(hash_value);
        }
    }

    bool contains(const string &item) {
        for (size_t i = 0; i < l; i++) {
            size_t hash_value = hash_string(item + char(('a' + i) % 26));
            if (filter.test(hash_value) == 0) return false;
        }
        return true;
    }
};

void get_error_probability(vector<string> &random_set,  CountingBloomFilter &bloom_filter) {
    int test_count = 1e5;
    long long int positive_error_quantity = 0;
    long long int negative_error_quantity = 0;
    vector<string> test = generate_random_strings(test_count);
    for (int i = 0; i < test_count; ++i) {
        if (bloom_filter.contains(test[i])) {
            if (!(find(random_set.begin(), random_set.end(), test[i]) != random_set.end())) {
                positive_error_quantity++;
            }
        }
        else {
            if (find(random_set.begin(), random_set.end(), test[i]) != random_set.end()) {
                negative_error_quantity++;
            }
        }
    }
    cout << "Positive error probability: " << (positive_error_quantity/test_count)*100 << " %" << endl;
    cout << "Positive error quantity: " << positive_error_quantity << endl;
    cout << "Negative error quantity: " << negative_error_quantity << endl;
}

int main() {
    size_t n = 1e6;
    double p = 0.01;

    CountingBloomFilter bloom_filter(n, p);

    int count = 1e5;
    vector<string> random_set = generate_random_strings(count);
    for (int i = 0; i < count; ++i) {
        bloom_filter.add(random_set[i]);
    }

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
                cout << "Y" << endl;
                if (find(random_set.begin(), random_set.end(), item) != random_set.end()) {
                    cout << "Was added before" << endl;
                }
                else {
                    cout << "Wasn't added before" << endl;
                }
            } else {
                cout << "N" << endl;
            }
        } else if (operation == '#') {
            return 0;
        } else {
            std::cerr << "Невідома операція: " << operation << std::endl;
        }
    }

    get_error_probability(random_set, bloom_filter);
    return 0;
}
