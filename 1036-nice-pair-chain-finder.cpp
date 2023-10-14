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
#include <thread>
#include <future>
#include <chrono>
using ull = long long;
using namespace std;

const int even_end = 2;
const int odd_end = 3;

bool is_perfect_square(long double x){
    if (x >= 0) {
        long long sr = sqrt(x);
        return (sr * sr == x);
    }
    return false;
}

std::atomic<bool> run1(true);
bool search_base_seq1(const vector<int>& seq0, const vector<bool>& seq0_parity_even, int here, vector<bool>& visited, vector<vector<int> >(&graph), vector<int>& seq1, vector<bool>& seq1_parity_even, const int goal_num){
    if(!run1){
        return false;
    }
    if(seq1.size() == goal_num){
        ofstream f;
        f.open("1036-nice-pair-chain.txt", ios::app);
        string str;
        str = "{";
        for(int j = 0;; ++j){
            if(j == seq1.size() - 1){
                str += to_string(seq1[j]);
                break;
            }
            str += to_string(seq1[j]) + ",";
        }
        str += "},\n";
        f.write(str.c_str(), str.size());
        f.close();
        return true;
    }

    if(graph[here].size() == 0) return false;

    bool cur_even = seq1.size() % 2 == 1;

    for(int i = 0; i < graph[here].size(); ++i){
        int next = graph[here][i];
        graph[next].clear();
        for(int j = 2; j*j <= 2*goal_num-1; ++j){
            int value = j*j - next;
            if(value >= 1 && value <= goal_num && value != next && !visited[value]){
                if(value == goal_num){
                    if(goal_num % 2 == 1 && seq0_parity_even[next]) graph[next].push_back(value);
                    if(goal_num % 2 == 0 && !seq0_parity_even[next]) graph[next].push_back(value);
                    continue;
                }
                if(next == goal_num){
                    if(goal_num % 2 == 1 && seq0_parity_even[value]) graph[next].push_back(value);
                    if(goal_num % 2 == 0 && !seq0_parity_even[value]) graph[next].push_back(value);
                    continue;
                }
                if(seq0_parity_even[next] != seq0_parity_even[value])
                    graph[next].push_back(value);
            } 
        }
    }

    sort(graph[here].begin(), graph[here].end(), [&](const int& next1, const int& next2){
                if(graph[next1].size() != graph[next2].size())
                    return graph[next1].size() < graph[next2].size(); //원래 < 이어야함
                return next1 > next2;
            });
    
    for(int i = 0; i < graph[here].size(); ++i){
        int next = graph[here][i];

        if(goal_num % 2 == 1 && next == odd_end && seq1.size() != goal_num - 1) continue;
        if(goal_num % 2 == 0 && next == even_end && seq1.size() != goal_num - 1) continue;
        if(next != goal_num && seq0_parity_even[next] != cur_even) continue; //same parity
        
        visited[next] = true;
        seq1.push_back(next);
        bool temp = seq1_parity_even[next];
        seq1_parity_even[next] = cur_even;

        if(search_base_seq1(seq0, seq0_parity_even, next, visited, graph, seq1, seq1_parity_even, goal_num)) return true;

        seq1_parity_even[next] = temp;
        seq1.pop_back();
        visited[next] = false;

        // You can't use a number that has failed once in a row. 
        // In other words, you can't use it until you visit a new valid number that forms a square sum with it.
        // Do not uncomment.
        // for(int k = 0; k < graph[here].size(); ++k){
        //     int next = graph[here][k];
        //     graph[next].clear();
        //     for(int j = 2; j*j <= 2*goal_num-1; ++j){
        //         int value = j*j - next;
        //         if(value >= 1 && value <= goal_num && value != next && !visited[value]){
        //             if(value == goal_num){
        //                 if(goal_num % 2 == 1 && seq0_parity_even[next]) graph[next].push_back(value);
        //                 if(goal_num % 2 == 0 && !seq0_parity_even[next]) graph[next].push_back(value);
        //                 continue;
        //             }
        //             if(next == goal_num){
        //                 if(goal_num % 2 == 1 && seq0_parity_even[value]) graph[next].push_back(value);
        //                 if(goal_num % 2 == 0 && !seq0_parity_even[value]) graph[next].push_back(value);
        //                 continue;
        //             }
        //             if(seq0_parity_even[next] != seq0_parity_even[value])
        //                 graph[next].push_back(value);
        //         } 
        //     }
        // }
    }

    return false;
}


//In the M1 clang environment, it was causing an "EXC_BAD_ACCESS (Code 2)" error when s0 exceeded 270
vector<int> s0 = 
{1,168,232,252,373,156,100,224,452,448,81,360,124,405,379,62,299,485,356,428,301,324,352,377,464,212,188,101,340,336,105,424,305,271,129,400,276,453,388,288,441,135,306,370,255,474,487,242,383,401,328,297,103,473,488,412,429,471,490,410,431,245,331,398,443,86,355,374,251,425,200,329,296,380,196,128,497,403,326,203,53,308,176,500,341,335,241,159,417,483,193,207,154,422,419,365,476,149,292,333,451,225,175,450,279,250,479,362,314,262,179,145,384,457,72,217,107,469,492,408,121,23,26,458,442,399,501,28,21,148,381,195,205,119,170,455,386,14,427,198,478,363,421,420,309,475,486,90,166,275,350,491,85,204,237,247,153,376,300,484,357,319,165,460,440,236,20,44,77,284,116,368,208,192,33,367,117,172,189,67,257,104,185,256,369,472,489,40,41,320,80,404,496,233,91,438,291,238,18,31,293,332,344,97,432,244,197,127,69,372,304,321,163,413,316,260,269,92,392,449,280,345,184,216,268,173,151,249,235,89,136,120,364,477,52,272,169,155,470,206,194,167,409,32,68,221,220,456,273,16,65,56,385,144,112,84,445,396,229,171,118,411,265,264,60,4,140,389,95,161,64,465,435,349,180,261,415,426,199,330,346,138,186,10,390,339,286,498,343,57,24,76,45,211,78,246,283,201,240,436,93,307,318,123,277,348,228,213,43,126,499,177,79,2,223,66,258,226,303,322,162,414,315,310,219,6,190,294,282,394,447,337,239,202,122,407,434,191,5,164,125,359,317,259,102,19,150,139,437,463,266,134,35,289,287,37,12,132,157,468,493,36,133,351,325,75,366,210,274,50,94,230,254,187,137,59,382,243,46,98,263,178,111,58,63,466,375,109,147,49,312,88,353,47,482,418,311,13,108,61,423,361,480,481,248,152,209,416,25,459,270,214,462,267,22,99,342,334,395,281,160,96,48,313,87,354,222,454,30,51,433,8,17,467,158,131,38,106,15,34,110,11,70,371,113,83,278,347,494,290,439,402,174,82,39,42,358,218,71,253,231,130,446,338,503,73,183,393,391,9,27,502,227,302,182,142,387,54,430,146,143,181,495,234,295,29,7,74,215,461,323,406,378,298,327,114,55,141,115,285,444,397,3};

void init_pos(vector<bool>& pos){
    pos.resize(s0.size()+1);
    for(int i = 0; i < s0.size(); ++i){
        pos[s0[i]] = i % 2 == 1 ? true : false;
    }
}

int main(int argc, char** argv){
    int seq0len=s0.size(), seq1len = seq0len+1;
    while(seq0len != 1037){
        cout << "NICE PAIR LEN : " << seq1len;
        vector<bool> p0;
        init_pos(p0);
        int call_state_changer;
        for(call_state_changer = 0; call_state_changer < 8; ++call_state_changer){
            vector<int> s1;
            s1.push_back(1);
            vector<bool> p1(seq1len + 1);
            p1[1] = false;
            vector<bool> v1(seq1len + 1, false);
            v1[1] = true;
            vector<vector<int> > g1(seq1len + 1);
            for(int j = 0; j*j <= 2*seq1len-1; ++j){
                int value = j*j - 1;

                if(value >= 1 && value <= seq1len - (seq1len/8)*call_state_changer) 
                    g1[1].push_back(value);
            }

            std::future<bool> rf = std::async(std::launch::async, search_base_seq1, std::ref(s0), std::ref(p0), 1, std::ref(v1), std::ref(g1), std::ref(s1), std::ref(p1), seq1len);
            std::future_status status = rf.wait_for(std::chrono::seconds(30));
            if (status == std::future_status::ready){
                if(rf.get()){
                    s0 = s1;
                    ++seq0len;
                    ++seq1len;
                    cout << " found!" << endl;
                    break;
                }
            }
            else{
                run1 = false;
                rf.wait();
                run1 = true;
            }
        }
        if(call_state_changer == 8){
            cout << " not found..." << endl;
            exit(1);
        }
    }
}
