#include <tuple>
#include <iostream>
#include <cmath>
#include <random>


#define PI 3.14159265

using namespace std;
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> shift;
uniform_real_distribution<double> leviDomain(-10.0, 10.0);
uniform_real_distribution<double> matyasDomain(-10.0, 10.0);

// Lévi function N.13
auto leviFunction = [](double x, double y){
    return (pow(sin(3*PI*x), 2)) +
            (pow(x-1, 2) * (1 + pow(sin(3*PI*y), 2))) +
            (pow(y-1, 2) * (1 + pow(sin(2*PI*y), 2)));
};

// Matyas function
auto matyasFunction = [](double x, double y){
    return 0.26 * (pow(x,2) + pow(y,2)) - 0.48 * x * y;
};

auto hill_climb = [](auto f1, double x, double y, int iteration) {
    double max_y = f1(x, y);
    for (int i = 1; i < iteration; i++) {
        double new_x = x + shift(gen);
        double new_y = y + shift(gen);

        double possible_best_y = f1(new_x, new_y);
        if (possible_best_y < max_y) {
            max_y = possible_best_y;
            x = new_x;
            y = new_y;
        }
    }
    return make_pair(x, y);
};


int main() {
    double from;
    double to;
    int choice;
    int iteration;
    cout << "Podaj start przesuniecia";
    cin >> from;
    cout << "Podaj stop przesuniecia";
    cin >> to;
    shift = uniform_real_distribution<double> (from, to);
    cout << "Wybierz funkcje:\n 1 Levi\n2 Matyas" << endl;
    cin >> choice;
    cout << "Podaj ilosc iteracji: " << endl;
    cin >> iteration;

    if (choice == 1) {
        auto [levi_x, levi_y] = make_pair(leviDomain(gen), leviDomain(gen));
        auto [result_x, result_y] = hill_climb(leviFunction, levi_x, levi_y, iteration);
        cout << "Best points:" << endl;
        cout << result_x << " : " << result_y << endl;
    } else if (choice == 2) {
        auto [matyas_x, matyas_y] = make_pair(matyasDomain(gen), matyasDomain(gen));
        auto [result_x, result_y] = hill_climb(matyasFunction, matyas_x, matyas_y, iteration);
        cout << "Best points:" << endl;
        cout << result_x << " : " << result_y << endl;
    }
    return 0;
}