#include <iostream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;

int32_t minimum(int32_t x, int32_t y) { return x < y ? x : y; }
int32_t maximum(int32_t x, int32_t y) { return x > y ? x : y; }

class RNG {
    public:
        uint32_t seed;
        
        RNG(uint32_t seed) {
            this->seed = seed;
        }
        
        uint32_t next() {
            uint32_t old = seed;
            
            uint32_t A = 1664525;
            uint32_t C = 1013904223;
            uint32_t R = (uint32_t)pow(2, 32);
            seed = (A * seed + C) % R;
            
            return old;
        }
};

class vEBT {
    public:
        int32_t u, min, max;
        vEBT *summary;
        vector<vEBT> clusters;
        
        vEBT(int32_t u) {
            int32_t m = sqrt(u);
            
            this->u = u;
            this->min = u;
            this->max = -1;
            
            if(u > 2) {
                this->summary = new vEBT(m);
                
                for(int i = 0; i < m; i++) {
                    vEBT c = vEBT(m);
                    this->clusters.push_back(c);
                }
            }
        }

        int32_t successor(int32_t x) {
            
            if(u == 2) {
                if(x == 0 && max == 1) { return 1; }
                else { return u; }
            } else if(min != u && x < min) {
                return min;
            }

            int32_t h = high(x);
            int32_t l = low(x);
            int32_t s;

            if(l < clusters[h].max) {
                s = clusters[h].successor(l);
                return index(h, s);
            
            } else {
                h = summary->successor(h);
                if(h == summary->u) {
                    return u;
                } else {
                    s = clusters[h].min;
                    return index(h, s);
                }
            }
        }
        
        int insert(int32_t x) {
            if(x == min || x == max) { return 0; }
            
            if(u == 2) {
                min = minimum(min, x);
                max = maximum(max, x);
                return 1;
            }

            if(min == u) {
                min = x;
                max = x;
                return 1;
            }

            if(x < min) {
                int32_t old = min;
                min = x;
                x = old;
            }
            
            max = maximum(max, x);
            int32_t h = high(x);
            int32_t l = low(x);

            if(clusters[h].min == clusters[h].u) {
                summary->insert(h);
            }
            
            int level = clusters[h].insert(l);
            return (level == 0) ? 0 : 1 + level;
        }

        int del(int32_t x) {
            
            if(u == 2) {
                int exists = 0;
                
                if(x == min) { 
                    exists = 1;
                    min = max != min ? max : u;
                }
                if(x == max) {
                    exists = 1;
                    max = min != u ? min : -1;
                }
                return exists;
            }

            int32_t i;
            if(x == min) {
                i = summary->min;
                if(i == summary->u) {
                    min = u;
                    max = -1;
                    return 1;
                } else {
                    min = index(i, clusters[i].min);
                    x = min;
                }
            }
            
            int32_t h = high(x);
            int32_t l = low(x);
            
            int level = clusters[h].del(l);
            if(level == 0) { return 0; }

            if(clusters[h].min == clusters[h].u) {
                summary->del(h);
            }

            if(x == max) {
                i = summary->max;
                if(i == -1) {
                    max = min;
                } else {
                    max = index(i, clusters[i].max);
                }
            }
            
            return 1 + level;
        }
        
        int32_t high(int32_t x) {
            return floor(x/sqrt(u));
        }
        
        int32_t low(int32_t x) {
            return x % (int32_t)sqrt(u);
        }
        
        int32_t index(int32_t h, int32_t l) {
            return h * sqrt(u) + l;
        }
};

int main() {

    uint32_t s, x, y;
    int m, b, n, i, f, d, p;
    string op;
    
    cin >> s >> m >> b >> n >> i >> f >> d >> p;
    
    RNG rng = RNG(s);
    int32_t u = pow(2, pow(2, m));
    vEBT t = vEBT(u);

    for(int j = 0; j < b; j++) {
        t.insert(rng.next() % u);
    }
    
    for(int j = 0; j < n; j++) {
        
        x = rng.next() % (i + f + d);

        if(x < i) { op = "INS"; }
        else if(i <= x && x < (i + f)) { op = "SUC"; }
        else { op = "DEL"; }
        
        if(op == "INS") {
            x = rng.next() % u;

            if(j % p == 0) {
                cout << "I " << t.insert(x) << "\n";
            } else {
                t.insert(x);
            }
        } else if(op == "SUC") {
            x = rng.next() % u;

            if(j % p == 0) {
                cout << "S " << t.successor(x) << "\n";
            } else {
                t.successor(x);
            }
        } else {
            y = rng.next() % u;
            x = t.successor(y);
            
            if(x == t.u) { x = y; }
            
            if(j % p == 0) {
                cout << "D " << t.del(x) << "\n";
            } else {
                t.del(x);
            }
        }
    }
    
    return 0;
}