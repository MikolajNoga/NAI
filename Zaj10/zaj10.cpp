#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>

using namespace std;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distr(1, 12);
std::uniform_int_distribution<> distrNodes(1, 8);

void writeToFile(string str) {
    ofstream file(R"(C:\Users\nogam\CLionProjects\NAI\Zaj10\graph.dot)");

    if (!file.is_open()) {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    file << str;

    file.close();
}

string getDotStr(){
    string str = "digraph path_graph {\n "
                "\trankdir=LR; \n"
                "\tnode [shape = circle];\n";
    for (int i = 1; i < 8; i++) {
        str += "\t" + to_string(i) + " -> " + to_string(i+1) + " [ label = \"" + to_string(distr(gen)) + "\" ];\n";
    }
    for (int i = 1; i < 8; i++) {
        str += "\t" + to_string(distrNodes(gen)) + " -> " + to_string(distrNodes(gen)) + " [ label = \"" + to_string(distr(gen)) + "\" ];\n";
    }
    str += "}";
    return str;
}



int main() {
    writeToFile(getDotStr());
    system(R"(dot C:\Users\nogam\CLionProjects\NAI\Zaj10\graph.dot -Tpng -o C:\Users\nogam\CLionProjects\NAI\Zaj10\image.png)");
    return 0;
}