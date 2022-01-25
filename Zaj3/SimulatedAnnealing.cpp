#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include <functional>
using namespace std;

#define M_PI 3.1415926535

random_device rd;
mt19937 gen(rd());

ostream& operator << (ostream& o, vector<double>v) {
    for (auto e : v) {
        o << e << ",";

    }
    return o;
}

vector<double> simulated_annealing(function<double(vector<double>)> f, function<bool(vector<double>)> f_domain, vector<double> p0, int iterations, function<vector<double>(vector<double>)> N, function<double(int)> T){
    auto currentPoint = p0;
    auto best = p0;

    uniform_real_distribution<> u_k(0.0, 1.0);

    if (!f_domain(currentPoint)) throw std::invalid_argument("The p0 point must be in domain");
    for (int i = 0; i < iterations; i++) {
        auto nextPoint = N(currentPoint);
        if (!f_domain(nextPoint)) continue;
        if (f(nextPoint) < f(currentPoint)) {
            currentPoint = nextPoint;
        }
        else {
            double u = u_k(gen);
            if (u < exp(-abs(f(nextPoint) - f(currentPoint)) / T(i)))
                currentPoint = nextPoint;
        }
        if (f(currentPoint) < f(best)) {
            best = currentPoint;
        }
        cout << (i + 1) << " " << f(currentPoint) <<endl;
    }
    return best;
}

vector<double> hillClimbing(function<double(vector<double>)> f, function<bool(vector<double>)> f_domain, vector<double> p0, vector <double> distanceBetweenPoints, int iterations){
    auto p = p0;
    double a = distanceBetweenPoints.at(0);
    double b = distanceBetweenPoints.at(1);
    uniform_int_distribution<> distrib(0, p.size() - 1);
    uniform_real_distribution<> distrib_r(a, b);
    if (!f_domain(p)) throw std::invalid_argument("The p0 point must be in domain");
    for (int i = 0; i < iterations; i++) {
        auto p2 = p;
        p2[distrib(gen)] += distrib_r(gen);
        if (f_domain(p2)) {
            double y2 = f(p2);
            if (y2 < f(p)) {
                p = p2;
            }
        }
        cout << (i + 1) << " " << f(p) <<endl;
    }
    return p;
}

// Lévi function N.13
auto leviFunction = [](vector<double> v){
    double x = v.at(0);
    double y = v.at(1);
    return (pow(sin(3*M_PI*x), 2)) +
           (pow(x-1, 2) * (1 + pow(sin(3*M_PI*y), 2))) +
           (pow(y-1, 2) * (1 + pow(sin(2*M_PI*y), 2)));
};

// Matyas function
auto matyasFunction = [](vector<double> v){
    double x = v.at(0);
    double y = v.at(1);
    return 0.26 * (pow(x,2) + pow(y,2)) - 0.48 * x * y;
};

//Jedna bo oba funkcje mają taką samą dziedzinę
auto functionDomain = [](vector<double> v) {
    return (abs(v[0]) <= 10) && (abs(v[1]) <= 10);
};

int main() {
    using namespace std;
    vector <double> numbers;
    int functionChoice, iterations, algorithmChoice;
    cout << "Choose function: (1)Levi, (2)Matyas\n";
    cin >> functionChoice;
    vector <double> shift{ -0.1 , 0.1 };

    if (functionChoice == 1) {
        uniform_real_distribution<> distrib_r(-10, 10);
        vector<double> leviP0 = { distrib_r(gen),distrib_r(gen) };
        cout << "Choose algorithm: (1)HillClimbing, (2)SimulatedAnnealing\n";
        cin >> algorithmChoice;
        cout << "Enter the number of iterations:\n";
        cin >> iterations;
        if (algorithmChoice == 1) {
            auto result = hillClimbing(leviFunction, functionDomain, leviP0, shift, iterations);
            cout << result << " : " << leviFunction(result) << endl;
        }
        else if (algorithmChoice == 2) {
            auto result = simulated_annealing(leviFunction,functionDomain,leviP0, iterations,[](auto p) {
                normal_distribution<double> n(0.0, 0.3);
                for (auto& e : p) {
                    e = e + n(gen);
                }
                return p;
                },[](int k) { return 1000.0 / k; });
            cout << result << " : " << leviFunction(result) << endl;
        }

    }
    else if (functionChoice == 2) {
        uniform_real_distribution<> distrib_r(-10, 10);
        vector<double> matyasP0 = { distrib_r(gen),distrib_r(gen) };
        cout << "Choose algorithm: (1)HillClimbing, (2)SimulatedAnnealing\n";
        cin >> algorithmChoice;
        cout << "Enter the number of iterations:\n" << endl;
        cin >> iterations;
        if (algorithmChoice == 1) {
            auto result = hillClimbing(matyasFunction, functionDomain, matyasP0, shift, iterations);
            cout << result << " : " << matyasFunction(result) << endl;
        }
        else if (algorithmChoice == 2) {
            auto result = simulated_annealing(matyasFunction, functionDomain, matyasP0, iterations, [](auto p) {
                normal_distribution<double> n(0.0, 0.3);
                for (auto& e : p) {
                    e = e + n(gen);
                }
                return p;
                },[](int k) { return 1000.0 / k; });
            cout << result << " : " << matyasFunction(result) << endl;
        }
    }
    return 0;
}