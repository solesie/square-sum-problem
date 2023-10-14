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
#include <map>
using ull = long long;
using namespace std;
const int MAX_LEN = 2048;
const int ROW25 = 1079; //1036
vector<int> S[ROW25];

const vector<string> dict = {"lnl", "ljh", "pn", "hj", "lj", "fd", "ZY", "dc", "jh", "XW", "TU", "fe", "XY", "aZ", "ln", "lh", "ab", "fh","aY","XV","VW","db","pr", "SR","QR"};

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
    readFile.open("1036-nice-pair-chain.txt");
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

void replace_all(string& str, const string& from, const string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}
void substitution(vector<string>& res){
    for(int s = 0; s < res.size(); ++s){
        for(int i = 0; i < dict.size(); ++i){
            char chr = i+'0';
            if(chr > '@')
                chr = i+23;
            string k(1,chr);
            replace_all(res[s], dict[i], k);
        }
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
    init_S();

    ofstream writeFile;
    writeFile.open("compressed-1036-nice-pair-chain.txt");
    string str;
    vector<string> res;
    for(int s = 0; s < ROW25; ++s){
        str = "\"";
        for(int i = 0; i < S[s].size() - 1; ++i){
            int squareRoot = sqrt(S[s][i] + S[s][i + 1]);
            assert(squareRoot <= 63);
            assert(squareRoot == sqrt(S[s][i] + S[s][i + 1]));
            str += toBase64(squareRoot);
        }
        str += "\",\n";
        res.push_back(str);
    }
    substitution(res);
    for(int s = 0; s < res.size(); ++s){
        writeFile.write(res[s].c_str(), res[s].size());
    }
    writeFile.close();  
}