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
#include <map>
#include <cmath>
#include <fstream>
#include <sstream>
using ll = long long;
using namespace std;

vector<int> S[1992];

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
    readFile.open("1036 Base Sequence.txt");
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

int main(){
    init_S();

    for(int i = 0; i < 1992; i += 2){
        int seq0_len = S[i].size();
        int seq1_len = S[i + 1].size();
        
        assert(seq0_len + 1 == seq1_len);

        bool seq0_appears_once = true;
        bool seq1_appears_once = true;
        bool all_same_parity_pos = true;
        vector<bool> seq0_visited(seq0_len + 1, false);
        vector<bool> seq1_visited(seq1_len + 1, false);
        vector<bool> seq0_even_pos(seq0_len + 1);
        vector<bool> seq1_even_pos(seq1_len + 1);

        for(int j = 0; j < seq0_len; ++j){
            int num = S[i][j];
            seq0_visited[num] = true;
            if(j%2 == 0) seq0_even_pos[num] = false;
            if(j%2 == 1) seq0_even_pos[num] = true;
        }
        for(int j = 0; j < seq1_len; ++j){
            int num = S[i + 1][j];
            seq1_visited[num] = true;
            if(j%2 == 0) seq1_even_pos[num] = false;
            if(j%2 == 1) seq1_even_pos[num] = true;
        }
        for(int j = 1; j <= seq0_len; ++j)
            if(!seq0_visited[j])
                seq0_appears_once = false;
        for(int j = 1; j <= seq1_len; ++j)
            if(!seq1_visited[j])
                seq1_appears_once = false;
        for(int j = 0; j < seq0_len; ++j){
            int num = S[i][j];
            if(seq0_even_pos[num] != seq1_even_pos[num])
                all_same_parity_pos = false;
        }
            
        assert(seq0_appears_once && seq1_appears_once);
        assert(all_same_parity_pos);
    }
    printf("success!\n");
}