#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <fstream>

#include "PreProcessor.hpp"
#include "helpers.hpp"
#include "initializer.hpp"

using namespace std;

void translate(string f) {
    ifstream file(f + ".pre");
    ofstream converted_file(f + ".s");
    initialize_file(converted_file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Erro" << endl;
    }
    else if (argc > 2) {
        cout << "Erro";
    }
    else {
        PreProcessor pre;
        pre.passage_zero(argv[1]);
        translate(argv[1]);
    }
    
    return 0;
}