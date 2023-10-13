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

// Set SEQ0_LEN to a value greater than or equal to 50.
// In the M1 clang environment, it was causing an "EXC_BAD_ACCESS (Code 2)" error when it exceeded 270, so I ran it in a Linux environment.
int SEQ0_LEN = 265;
int SEQ1_LEN = SEQ0_LEN + 1;
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
        f.open("1036-nice-pair2.txt", ios::app);
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
                    return graph[next1].size() < graph[next2].size();
                //If you're not getting an answer when seq0_len is small, you can change the direction of the inequality.
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

    return false;
}


std::atomic<bool> run0(true);
bool search_base_seq0(int here, vector<bool>& visited, vector<vector<int> >(&graph), vector<int>& seq0, vector<bool>& seq0_parity_even, const int goal_num, vector<int>& seq1, vector<bool>& seq1_parity_even){
    if(!run0){
        return false;
    }
    if(seq0.size() == goal_num){
        vector<bool> v(goal_num + 2, false);
        v[1] = true;
        vector<vector<int> > g(goal_num + 2);
        for(int j = 2; j*j <= 2*SEQ1_LEN-1; ++j){
            int value = j*j - 1;
            if(value >= 1 && value <= SEQ1_LEN && seq0_parity_even[1] != seq0_parity_even[value]) 
                g[1].push_back(value);
        }
        seq1.clear();
        seq1.push_back(1);
        seq1_parity_even.resize(goal_num + 2);
        seq1_parity_even[1] = false;

        std::future<bool> result_future = std::async(std::launch::async, search_base_seq1, std::ref(seq0), std::ref(seq0_parity_even), 1, std::ref(v), std::ref(g), std::ref(seq1), std::ref(seq1_parity_even), goal_num + 1);
        std::future_status status = result_future.wait_for(std::chrono::seconds(2));
        if (status == std::future_status::ready){
            if(result_future.get()) 
                return true;
        }
        else{
            run1 = false;
            result_future.wait();
            run1 = true;
        }
        return false;
    }

    if(graph[here].size() == 0) return false;

    bool cur_even = seq0.size() % 2 == 1;

    for(int i = 0; i < graph[here].size(); ++i){
        int next = graph[here][i];
        graph[next].clear();
        for(int j = 2; j*j <= 2*goal_num-1; ++j){
            int value = j*j - next;
            if(value >= 1 && value <= goal_num && value != next && !visited[value]) 
                graph[next].push_back(value);
        }
    }
    sort(graph[here].begin(), graph[here].end(), [&](const int& next1, const int& next2){
                if(graph[next1].size() != graph[next2].size())
                    return graph[next1].size() < graph[next2].size();
                //If you're not getting an answer when seq0_len is small, you can change the direction of the inequality.
                return next1 > next2;
            });
    
    for(int i = 0; i < graph[here].size(); ++i){
        int next = graph[here][i];

        if(goal_num % 2 == 1 && next == odd_end && seq0.size() != goal_num - 1) continue;
        if(goal_num % 2 == 0 && next == even_end && seq0.size() != goal_num - 1) continue;
        if(goal_num % 2 == 1 && next == even_end && !cur_even) continue;
        if(goal_num % 2 == 0 && next == odd_end && cur_even) continue;
        
        visited[next] = true;
        seq0.push_back(next);
        bool temp = seq0_parity_even[next];
        seq0_parity_even[next] = cur_even;

        if(search_base_seq0(next, visited, graph, seq0, seq0_parity_even, goal_num, seq1, seq1_parity_even)) 
            return true;

        seq0_parity_even[next] = temp;
        seq0.pop_back();
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
    return false;
}

int main(int argc, char** argv){

    while(SEQ0_LEN != 1036){
        cout << "NICE PAIR LEN : " << SEQ0_LEN;
        int call_state_changer;
        for(call_state_changer = 3; call_state_changer >= 0; --call_state_changer){
            vector<bool> v(SEQ0_LEN + 1, false);
            v[1] = true;
            vector<vector<int> > g(SEQ0_LEN + 1);
            for(int j = 2; j*j <= 2*SEQ0_LEN-1; ++j){
                int value = j*j - 1;
                if(value >= 1 && value <= SEQ0_LEN - (SEQ0_LEN/2)*call_state_changer) 
                    g[1].push_back(value);
            }
            vector<int> s0;
            s0.push_back(1);
            vector<bool> p0(SEQ0_LEN + 1);
            p0[1] = false;
            vector<int> s1;
            vector<bool> p1;

            std::future<bool> result_future = std::async(std::launch::async, search_base_seq0, 1, std::ref(v), std::ref(g), std::ref(s0), std::ref(p0), SEQ0_LEN, std::ref(s1), std::ref(p1));
            //If you're not getting an answer, you can increase this time.
            std::future_status status = result_future.wait_for(std::chrono::seconds(20));
            if (status == std::future_status::ready){
                if(result_future.get()){
                    cout << " found!" << endl;
                    break;
                }
            }
            else{
                run0 = false;
                run1 = false;
                result_future.wait();
                run0 = true;
                run1 = true;
            }
        }
        if(call_state_changer == -1){
            cout << " not found..." << endl;
            break;
        }
        else{
            ++SEQ0_LEN;
            ++SEQ1_LEN;
        }
    }
}
