#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

#include "Line.hpp"

using namespace std;

const vector<string> split(const string &s, const char &c);

string remove_initial_spaces(string in);

string remove_final_spaces(string in);

string remove_unecessary_spaces(string in);

string format_line(string line);

Line token_separator(string input);

#endif