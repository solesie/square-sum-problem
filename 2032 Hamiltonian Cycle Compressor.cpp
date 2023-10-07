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
using ll = long long;
using namespace std;
const int MAX_LEN = 2048;
const int ROW49 = 3983;
const int ROW25 = 1991; //1036
int S[3984][2048];

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

void initSeq(){
    ifstream readFile;
    readFile.open("2032 Hamiltonian Cycles.txt");
    if (readFile.is_open()){
        int row = 0;
        while (!readFile.eof()){
            string str;
            getline(readFile, str);
            string sub = str.back() == '}' ? str.substr(1, str.size() - 1) : str.substr(1, str.size() - 2);
            vector<string> splitted = split(sub, ',');
            for(int i = 0; i < splitted.size(); ++i)
                S[row][i] = stoi(splitted[i]);
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
int fromBase64(const std::string& base64_str) {
    const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int result = 0;
    int power = 1;

    for (int i = base64_str.length() - 1; i >= 0; --i) {
        char c = base64_str[i];
        int value = base64_chars.find(c);
        result += value * power;
        power *= 64;
    }

    return result;
}

int main(){
    initSeq();

    ofstream writeFile;
    writeFile.open("Compressed 2032 Hamiltonian Cycle.txt");
    string str;
    vector<string> res;
    for(int s = 0; s < ROW25; s += 2){
        str = "";
        for(int i = 0; i < MAX_LEN - 1; ++i){
            if(S[s][i + 1] == 0) break;
            int squareRoot = sqrt(S[s][i] + S[s][i + 1]);
            assert(squareRoot <= 63);
            assert(squareRoot == sqrt(S[s][i] + S[s][i + 1]));
            str += toBase64(squareRoot);
        }
        str += "\n";
        res.push_back(str);
    }
    // sort(res.begin(), res.end());
    for(int s = 0; s < res.size(); ++s){
        writeFile.write(res[s].c_str(), res[s].size());
    }
    writeFile.close();  
}