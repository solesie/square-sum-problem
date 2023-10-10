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
using ll = long long;
using namespace std;

// Set SEQ0_LEN to a value greater than or equal to 50.
// In the M1 clang environment, it was causing an "EXC_BAD_ACCESS (Code 2)" error when it exceeded 270, so I ran it in a Linux environment.
int SEQ0_LEN = 71; // change this
int SEQ1_LEN = 72; // change this(SEQ0_LEN+1)
const int even_end = 2;
const int odd_end = 3;

bool is_perfect_square(long double x){
    if (x >= 0) {
        long long sr = sqrt(x);
        return (sr * sr == x);
    }
    return false;
}
std::atomic<bool> run(true);
bool search_base_seq(const bool first_call, const vector<int>& seq0, const vector<bool>& seq0_parity_pos, int here, vector<bool>& visited, vector<vector<int> >(&graph), vector<int>& incoming_cnt, vector<int>& seq1, vector<bool>& seq1_parity_pos, const int goal_num){
    if(!run){
        return false;
    }
    if(seq1.size() == goal_num){
        if(goal_num == SEQ1_LEN){
            ofstream f;
            f.open("1036 Base Sequence.txt", ios::app);
            string str;
            str = "{";
            for(int j = 0; j < seq0.size(); ++j){
                if(j == seq0.size() - 1){
                    str += to_string(seq0[j]);
                    break;
                }
                str += to_string(seq0[j]) + ",";
            }
            str += "},\n";
            f.write(str.c_str(), str.size());
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
        vector<bool> v(goal_num + 2, false);
        v[1] = true;
        vector<vector<int> > g(goal_num + 2);
        vector<int> i_c(goal_num + 2, 0);
        for(int j = 2; j*j <= 2*SEQ1_LEN-1; ++j){
            int value = j*j - 1;
            if(value >= 1 && value <= SEQ1_LEN && seq1_parity_pos[1] != seq1_parity_pos[value]) g[1].push_back(value);
        }
        vector<int> s;
        s.push_back(1);
        vector<bool> p(goal_num + 2);
        p[1] = false;

        std::future<bool> result_future = std::async(std::launch::async, search_base_seq, false, std::ref(seq1), std::ref(seq1_parity_pos), 1, std::ref(v), std::ref(g), std::ref(i_c), std::ref(s), std::ref(p), goal_num + 1);
        std::future_status status = result_future.wait_for(std::chrono::seconds(2));
        if (status == std::future_status::ready){
            if(result_future.get()) 
                return true;
        }
        else{
            run = false;
            result_future.wait();
            run = true;
        }

        // if(search_base_seq(false, seq1, seq1_parity_pos, 1, v, g, i_c, s, p, goal_num + 1))
        //     return true;
        return false;
    }

    if(graph[here].size() == 0) return false;

    bool even_pos = seq1.size() % 2 == 1;

    for(int i = 0; i < graph[here].size(); ++i){
        int next = graph[here][i];
        graph[next].clear();
        if(first_call){
            for(int j = 2; j*j <= 2*goal_num-1; ++j){
                int value = j*j - next;
                if(value >= 1 && value <= goal_num && value != next && !visited[value]) graph[next].push_back(value);
            }
        }
        else{
            for(int j = 2; j*j <= 2*goal_num-1; ++j){
                int value = j*j - next;
                if(value >= 1 && value <= goal_num && value != next && !visited[value]){
                    if(value == goal_num){
                        if(goal_num % 2 == 1 && seq0_parity_pos[next]) graph[next].push_back(value);
                        if(goal_num % 2 == 0 && !seq0_parity_pos[next]) graph[next].push_back(value);
                        continue;
                    }
                    if(next == goal_num){
                        if(goal_num % 2 == 1 && seq0_parity_pos[value]) graph[next].push_back(value);
                        if(goal_num % 2 == 0 && !seq0_parity_pos[value]) graph[next].push_back(value);
                        continue;
                    }
                    if(seq0_parity_pos[next] != seq0_parity_pos[value])
                        graph[next].push_back(value);
                } 
            }
        }
    }
    sort(graph[here].begin(), graph[here].end(), [&](const int& next1, const int& next2){
                if(graph[next1].size() != graph[next2].size())
                    return graph[next1].size() < graph[next2].size();
                return next1 > next2;
            });

    //backtrack
    //I dont know this is valid...
    for(int i = 0; i < graph[here].size(); ++i){
        int next = graph[here][i];

        --incoming_cnt[next];
        
        if(incoming_cnt[next] == 0 && !visited[next] && seq1.size() != goal_num - 1){
            for(int k = 0; k <= i; ++k) ++incoming_cnt[graph[here][k]];
            return false;
        }
    }
    
    for(int i = 0; i < graph[here].size(); ++i){
        int next = graph[here][i];

        if(goal_num % 2 == 1 && next == odd_end && seq1.size() != goal_num - 1) continue;
        if(goal_num % 2 == 0 && next == even_end && seq1.size() != goal_num - 1) continue;
        if(!first_call && next != goal_num && seq0_parity_pos[next] != even_pos) continue; //same parity
        if(first_call){
            if(goal_num % 2 == 1 && next == even_end && !even_pos) continue;
            if(goal_num % 2 == 0 && next == odd_end && even_pos) continue;
        }
        
        visited[next] = true;
        seq1.push_back(next);
        bool temp = seq1_parity_pos[next];
        seq1_parity_pos[next] = even_pos;

        if(search_base_seq(first_call, seq0, seq0_parity_pos, next, visited, graph, incoming_cnt, seq1, seq1_parity_pos, goal_num)) return true;

        seq1_parity_pos[next] = temp;
        seq1.pop_back();
        visited[next] = false;

        // You can't use a number that has failed once in a row. 
        // In other words, you can't use it until you visit a new valid number that forms a square sum with it.
        // Do not uncomment.
        // if(!first_call){
        //     for(int k = 0; k < graph[here].size(); ++k){
        //         int next = graph[here][k];
        //         graph[next].clear();
        //         if(first_call){
        //             for(int j = 2; j*j <= 2*goal_num-1; ++j){
        //                 int value = j*j - next;
        //                 if(value >= 1 && value <= goal_num && value != next && !visited[value]) graph[next].push_back(value);
        //             }
        //         }
        //         else{
        //             for(int j = 2; j*j <= 2*goal_num-1; ++j){
        //                 int value = j*j - next;
        //                 if(value >= 1 && value <= goal_num && value != next && !visited[value] && (next == goal_num || value == goal_num || seq0_parity_pos[next] != seq0_parity_pos[value])) 
        //                     graph[next].push_back(value);
        //             }
        //         }
        //     }
        // }
    }

    for(int j = 0; j < graph[here].size(); ++j) ++incoming_cnt[graph[here][j]];
    return false;
}

int main(int argc, char** argv){
    while(SEQ0_LEN != 1036){
        vector<int> dummy1;
        cout << "SEQ0_LEN : " << SEQ0_LEN;
        vector<bool> v(SEQ0_LEN + 1, false);
        v[1] = true;
        vector<vector<int> > g(SEQ0_LEN + 1);
        vector<int> i_c(SEQ0_LEN + 1, 0);
        for(int j = 2; j*j <= 2*SEQ0_LEN-1; ++j){
            int value = j*j - 1;
            // Choose one of the following three.
            // Surprisingly, if this method is refactored, I'm confident that you can find 1036 nice pairs within 5 minutes.
            // if(value >= 1 && value <= SEQ0_LEN) {
            //     g[1].push_back(value);
            // }
            // if(value >= 1 && value <= SEQ0_LEN - SEQ0_LEN/2) {
            //     g[1].push_back(value);
            // }
            // if(value >= 1 && value <= SEQ0_LEN - SEQ0_LEN/4) {
            //     g[1].push_back(value);
            // }
        }
        
        vector<int> s;
        s.push_back(1);
        vector<bool> p(SEQ0_LEN + 1);
        p[1] = false;
        vector<bool> dummy2(SEQ0_LEN + 1);
        if(search_base_seq(true, dummy1, dummy2, 1, v, g, i_c, s, p, SEQ0_LEN)){
            cout << " found!" << endl;
            ++SEQ0_LEN;
            ++SEQ1_LEN;
        }
        else{
            cout << " not found..." << endl;
            ++SEQ0_LEN;
            ++SEQ1_LEN;
        }
    }
}
