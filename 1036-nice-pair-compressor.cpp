#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <string>
#include <memory.h>
#include <limits.h>
#include <cassert>
#include <fstream>
#include <cmath>
#include <sstream>
using ull = long long;
using namespace std;
const int MAX_LEN = 2048;
const int ROW49 = 3983;
const int ROW25 = 1992; //1036
vector<int> S[ROW25];

int findIdx(int s, int val){
    for(int i = 0; i < MAX_LEN; ++i){
        if(S[s][i] == val) return i;
    }
    return val;
}

vector<string> split (const std::string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

void init_S(){
    ifstream readFile;
    readFile.open("1036-nice-pair.txt");
    if (readFile.is_open()){
        int row = 0;
        while (!readFile.eof()){
            string str;
            getline(readFile, str);
            string sub = str.substr(1, str.size() - 2);
            vector<string> splitted = split(sub, ',');
            for(int i = 0; i < splitted.size(); ++i)
                S[row].push_back(stoi(splitted[i]));
            ++row;
        }
        readFile.close();
    }
}

std::string toBase64(unsigned long long number) {
    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string result;
    while (number > 0) {
        result = base64_chars[number % 64] + result;
        number /= 64;
    }

    return result;
}

int main(){
    init_S();

    ofstream writeFile;
    writeFile.open("compressed-1036-nice-pair.txt");
    string str;
    vector<string> res;
    for(int s = 0; s < ROW25; ++s){
        str = "";
        for(int i = 0; i < S[s].size() - 1; ++i){
            int squareRoot = sqrt(S[s][i] + S[s][i + 1]);
            assert(squareRoot <= 63);
            assert(squareRoot == sqrt(S[s][i] + S[s][i + 1]));
            str += toBase64(squareRoot);
        }
        str += "\n";
        res.push_back(str);
    }
    for(int s = 0; s < res.size(); ++s){
        writeFile.write(res[s].c_str(), res[s].size());
    }
    writeFile.close();  
}