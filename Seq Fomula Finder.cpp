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
using ll = long long;
using namespace std;

const int start = 1;
const int even_end = 2;
const int odd_end = 3;
const int min_val = 25*min(start, min(even_end, odd_end)) - 12;
const int max_val = 25*max(start, max(even_end, odd_end)) + 12;
struct term{
    bool even;
    int c, in_order;
    bool l;
};

bool isPerfectSquare(long double x){
    if (x >= 0) {
        long long sr = sqrt(x);
        return (sr * sr == x);
    }
    return false;
}

pair<int, int> find_endpoints(term& term){
    bool even = term.even, l = term.l;
    int c = term.c, in_order = term.in_order;
    if(in_order == 0) return {c, c};
    even ^= l;
    int f, s;
    if(even)
        f = 25*start + c, s = 25*even_end - c;
    else
        f = 25*start + c, s = 25*odd_end + c;
    if(in_order == -1) swap(f, s);
    return {f, s};
}

int get_terms_idx(int c, bool is_rev){
    return 2*c + is_rev + 37;
}
int get_c(int index){
    return (index - 37 - 1) / 2;
}
int get_rev(int index){
    if(1 <= index && index <= 12) return index;
    if(index % 2 == 1) return index + 1;
    return index - 1;
}
term even_terms[37][1 + 12 + 50];
term odd_terms[37][1 + 12 + 50];
vector<int> even_graph[37][1 + 12 + 50];
vector<int> odd_graph[37][1 + 12 + 50];
int even_graph_coming_cnt[37][1 + 12 + 50];
int odd_graph_coming_cnt[37][1 + 12 + 50];
void init(){
    for(int res = 12; res <= 36; ++res){
        for(int c = -12; c <= 12; ++c){
            //n: even
            //25(n+1)+c <= 25n+res
            if(25 + c <= res){
                even_terms[res][get_terms_idx(c, false)] = {true, c, 1, true}; //shift(25L, c)
                even_terms[res][get_terms_idx(c, true)] = {true, c, -1, true}; //rev(shift(25L, c))
            }
            else{
                even_terms[res][get_terms_idx(c, false)] = {true, c, 1, false}; //shift(25S, c)
                even_terms[res][get_terms_idx(c, true)] = {true, c, -1, false}; //rev(shift(25S, c))
            }
            //n: odd
            if(25 - c <= res){
                odd_terms[res][get_terms_idx(c, false)] = {false, c, 1, true};
                odd_terms[res][get_terms_idx(c, true)] = {false, c, -1, true};
            }
            else{
                odd_terms[res][get_terms_idx(c, false)] = {false, c, 1, false};
                odd_terms[res][get_terms_idx(c, true)] = {false, c, -1, false};
            }
        }
    }
    for(int res = 12; res <= 36; ++res)
        for(int i = 1; i <= 12; ++i)
            even_terms[res][i] = odd_terms[res][i] = {0, i, 0, 0};

    for(int res = 12; res <= 36; ++res){
        for(int i = 1; i < 63; ++i){
            for(int j = 1; j < 63; ++j){
                if(1 <= i && i <= 12){
                    if(i == j) continue;
                    if(j <= 12 && isPerfectSquare(i + j)){
                        even_graph[res][i].push_back(j);
                        odd_graph[res][i].push_back(j);
                        ++even_graph_coming_cnt[res][j];
                        ++odd_graph_coming_cnt[res][j];
                    }
                    if(j > 13){
                        if(isPerfectSquare(i + find_endpoints(even_terms[res][j]).first)){
                            even_graph[res][i].push_back(j);
                            ++even_graph_coming_cnt[res][j];
                        }
                        if(isPerfectSquare(i + find_endpoints(odd_terms[res][j]).first)){
                            odd_graph[res][i].push_back(j);
                            ++odd_graph_coming_cnt[res][j];
                        }
                    }
                }
                else{
                    if(j > 12 && get_c(i) == get_c(j)) continue;
                    if(isPerfectSquare(find_endpoints(even_terms[res][i]).second + find_endpoints(even_terms[res][j]).first)){
                        even_graph[res][i].push_back(j);
                        ++even_graph_coming_cnt[res][j];
                    }
                    if(isPerfectSquare(find_endpoints(odd_terms[res][i]).second + find_endpoints(odd_terms[res][j]).first)){
                        odd_graph[res][i].push_back(j);
                        ++odd_graph_coming_cnt[res][j];
                    }
                }
            }
        }
    }
}

bool is_next_pos_even(bool cur_pos_even, term& t){
    bool ret;
    if(cur_pos_even){
        if(t.even && t.l == 0 || !t.even && t.l == 1) ret = true;
        else ret = false;
    }
    else{
        if(t.in_order == 0 || t.even && t.l == 1 || !t.even && !t.l == 0) ret = true;
        else ret = false;
    }
    return ret;
}

bool can_locate(const bool even, bool prev_parity, term& t1, bool cur_parity, term&t2){
    assert(t1.c == t2.c);
    if(t1.in_order == 0 && t2.in_order == 0){
        if(prev_parity == cur_parity) return true;
        return false;
    }
    if(even){
        //S를 그대로 쓰는 경우
        if(t1.l == 0 && t2.l == 0){
            if(t1.in_order == t2.in_order && prev_parity == cur_parity) return true;
            if(t1.in_order != t2.in_order && prev_parity != cur_parity) return true;
            return false;
        }
        //S->L로 바꾸는 경우
        if(t1.l == 0 && t2.l == 1){
            if(t1.in_order == 1 && prev_parity == cur_parity) return true;
            if(t1.in_order == -1 && prev_parity != cur_parity) return true;
            return false;
        }
        //L을 그대로 쓰는 경우
        if(t1.l == 1 && t2.l == 1){
            if(prev_parity == cur_parity) return true;
            return false;
        }
        throw exception();
    }
    else{
        //S를 그대로 쓰는 경우
        if(t1.l == 0 && t2.l == 0){
            if(prev_parity == cur_parity) return true;
            return false;
        }
        //S->L로 바꾸는 경우
        if(t1.l == 0 && t2.l == 1){
            if(t2.in_order == 1 && prev_parity == cur_parity) return true;
            if(t2.in_order == -1 && prev_parity != cur_parity) return true;
            return false;
        }
        //L을 그대로 쓰는 경우
        if(t1.l == 1 && t2.l == 1){
            if(t1.in_order == t2.in_order && prev_parity == cur_parity) return true;
            if(t1.in_order != t2.in_order && prev_parity != cur_parity) return true;
            return false;
        }
        throw exception();
    }
}

//fomula[][][0] = c
//fomula[][][1] = 0:상수, 1:순서대로, -1:역순
//fomula[][][2] = shift(k^2 * s, c) : 0, shift(k^2 * l, c) : 1
int fomula[4*37][25+25/2+1][4];

bool can_expand_to_nicepair(const bool even, const int res, bool even_pos, int idx, vector<bool>& visited,const vector<pair<int, int> >& perm1, vector<pair<int, int> >& perm2){
    if(perm2.size() == 25 + 12){
        // validate_fomula(perm2);
        for(int i = 0; i < perm1.size(); ++i){
            if(1 <= perm1[i].first && perm1[i].first <= 12) 
                printf("{%d},", perm1[i].first);
            else{
                pair<int, int> p = even ? find_endpoints(even_terms[res][perm1[i].first]) : find_endpoints(odd_terms[res][perm1[i].first]);
                printf("{%d,%d},", p.first, p.second);
            }
        }
        cout << endl;
        for(int i = 0; i < perm2.size(); ++i){
            if(1 <= perm2[i].first && perm2[i].first <= 12) 
                printf("{%d},", perm2[i].first);
            else{
                pair<int, int> p = even ? find_endpoints(even_terms[res][perm2[i].first]) : find_endpoints(odd_terms[res][perm2[i].first]);
                printf("{%d,%d},", p.first, p.second);
            }
        }
        cout << endl;
        return true;
    }
    vector<int>(&graph)[37][1 + 12 + 50] = even ? even_graph : odd_graph;
    term(&terms)[37][1 + 12 + 50] = even ? even_terms : odd_terms;

    for(int i = 0; i < graph[res][idx].size(); ++i){
        int next = graph[res][idx][i];
        int next_rev = get_rev(next);
        
        if(visited[next] || (next > 12 && visited[next_rev])) continue;
        if(even && res % 2 == 1 || !even && res % 2 == 0){ //만들려는게 홀수
            if(next == odd_end && perm2.size() != 36) continue; 
        }
        else{
            if(next == even_end && perm2.size() != 36) continue;
        }
        
        for(int j = 0; j < perm1.size(); ++j){
            if(terms[res-1][perm1[j].first].c == terms[res][next].c){
                if(terms[res-1][perm1[j].first].in_order == 0 || terms[res][next].in_order == 0){
                    if(!(terms[res-1][perm1[j].first].in_order == 0 && terms[res][next].in_order == 0)) continue;
                }
                term& prev = terms[res-1][perm1[j].first];
                bool prev_even = perm1[j].second;
                term& cur = terms[res][next];
                if(can_locate(even, prev_even, prev, even_pos, cur)){
                    visited[next] = true;
                    perm2.push_back({next, even_pos});

                    if(can_expand_to_nicepair(even, res, is_next_pos_even(even_pos, cur), next, visited, perm1, perm2)) return true;

                    perm2.pop_back();
                    visited[next] = false;
                }
            }
        }
    }
    return false;
}

//(n,n+1) -> (25n+res,25n+res+1) 로 확장가능한 공식이 있는가?
bool exist_nicepair_fomula(const bool even, const int res, bool even_pos, int idx, vector<bool>& visited, int (&coming_cnt)[63], vector<pair<int, int> >& perm1){
    if(perm1.size() == 25 + 12){
        // validate_fomula(perm1);

        vector<bool> visited(63, false);
        visited[1] = true;
        vector<pair<int, int> > perm2;
        perm2.push_back({1, false});
        // int coming_cnt[1 + 12 + 50];
        // if(even)
        //     copy(begin(even_graph_coming_cnt[res+1]), end(even_graph_coming_cnt[res+1]), begin(coming_cnt));
        // else
        //     copy(begin(odd_graph_coming_cnt[res+1]), end(odd_graph_coming_cnt[res+1]), begin(coming_cnt));
        if(can_expand_to_nicepair(even, res + 1, true, 1, visited, perm1, perm2)) return true;
        return false;
    }

    vector<int>(&graph)[37][1 + 12 + 50] = even ? even_graph : odd_graph;
    term(&terms)[37][1 + 12 + 50] = even ? even_terms : odd_terms;

    //here을 옴으로서 here_rev는 들릴 수 없게 되었다.
    //here_rev로만 갈 수 있는 곳을 못가게 된다면 안된다.
    if(idx >= 13){
        int rev = get_rev(idx);
        for(int j = 0; j < graph[res][rev].size(); ++j){
            --coming_cnt[graph[res][rev][j]];
            if(!visited[graph[res][rev][j]] && !visited[get_rev(graph[res][rev][j])] 
            && coming_cnt[graph[res][idx][j]] == 0 && coming_cnt[get_rev(graph[res][rev][j])] == 0 && perm1.size() != 36){
                for(int k = 0; k <= j; ++k) ++coming_cnt[graph[res][rev][k]];
                return false;
            }
        }
    }

    for(int i = 0; i < graph[res][idx].size(); ++i){
        int next = graph[res][idx][i];
        int next_rev = get_rev(next);

        --coming_cnt[next];
        
        if(visited[next] || (next > 12 && visited[next_rev])) continue;
        if(even && res % 2 == 1 || !even && res % 2 == 0){ //만들려는게 홀수
            if(next == odd_end && perm1.size() != 36) continue; //3은 비워둬야함
            if(next == even_end && !even_pos) continue; //2는 짝수번째에 와야함
        }
        else{
            if(next == even_end && perm1.size() != 36) continue;
            if(next == odd_end && even_pos) continue; //3은 홀수번째에 와야함
        }

        if(coming_cnt[next] == 0 && coming_cnt[get_rev(next)] == 0 
        &&!visited[next] && !visited[get_rev(next)] && perm1.size() != 36){
            visited[next] = true;
            perm1.push_back({next, even_pos});

            if(exist_nicepair_fomula(even, res, is_next_pos_even(even_pos, terms[res][next]), next, visited, coming_cnt, perm1)) 
                return true;
            for(int k = 0; k <= i; ++k) ++coming_cnt[graph[res][idx][k]];
            if(idx >= 13){
                int here_rev = get_rev(idx);
                for(int j = 0; j < graph[res][here_rev].size(); ++j){
                    ++coming_cnt[graph[res][here_rev][j]];
                }
            }
            visited[next] = false;
            perm1.pop_back();
            return false;
        }
        
        visited[next] = true;
        perm1.push_back({next, even_pos});

        if(exist_nicepair_fomula(even, res, is_next_pos_even(even_pos, terms[res][next]), next, visited, coming_cnt, perm1)) return true;

        perm1.pop_back();
        visited[next] = false;
    }

    for(int j = 0; j < graph[res][idx].size(); ++j){
        ++coming_cnt[graph[res][idx][j]];
    }
    if(idx >= 13){
        int here_rev = get_rev(idx);
        for(int j = 0; j < graph[res][here_rev].size(); ++j){
            ++coming_cnt[graph[res][here_rev][j]];
        }
    }
    return false;
}

int main(int argc, char** argv){
    init();
    for(int is_even_n = 0; is_even_n <= 1; ++is_even_n){
        if(is_even_n)
            printf("n is even\n");
        else
            printf("n is odd\n");
        for(int res = 12; res <= 36; ++res){
            cout <<"res "<< res <<" : "<<endl;
            vector<bool> visited(63, false);
            visited[1] = true;
            vector<pair<int, int> > perm1;
            perm1.push_back({1, true});
            int coming_cnt[1 + 12 + 50];
            if(is_even_n)
                copy(begin(even_graph_coming_cnt[res]), end(even_graph_coming_cnt[res]), begin(coming_cnt));
            else
                copy(begin(odd_graph_coming_cnt[res]), end(odd_graph_coming_cnt[res]), begin(coming_cnt));
            exist_nicepair_fomula(is_even_n, res, 1, true, visited, coming_cnt, perm1);
        }
    }
}