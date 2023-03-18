#include <iostream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;

uint32_t minimum(int x, int y) { return x < y ? x : y; }
uint32_t maximum(int x, int y) { return x > y ? x : y; }

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
        int u, min, max;
        vEBT *summary;
        vector<vEBT> clusters;
        
        vEBT(int u) {
            int m = sqrt(u);
            
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
        
        int insert(int x) {
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
                int oldMin = min;
                min = x;
                x = oldMin;
            }
            max = maximum(max, x);
            int h = high(x);
            int l = low(x);
            
            if(clusters[h].min == clusters[h].u) {
                summary->insert(h);
            }
            return 1 + clusters[h].insert(l);
        }
        
        int successor(int x) {
            if(u == 2) {
                if(x == 0 && max == 1) { return 1; }
                else { return -1; }
            
            } else if(min != -1 && x < min) {
                return min;
            } else {
                int h = high(x);
                int l = low(x);
                int m = clusters[h].max;
                int offset, s;
                
                if(m != -1 && l < m) {
                    offset = clusters[h].successor(l);
                    return index(h, offset);
                } else {
                    s = summary->successor(h);
                    if(s == -1) {
                        return -1;
                    } else {
                        offset = clusters[s].min;
                        return index(s, offset);
                    }
                }
            }
        }
        
        int del(int x) {
            if(u == 2) {
                if(x == min) {
                    min = max != min ? max : u;
                }
                if(x == max) {
                    max = min != u ? min : -1;
                }
                return 1;
            }
            
            int i;
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
            
            int h = high(x);
            int l = low(x);
            
            int level = 1 + clusters[h].del(l);
            if(clusters[h].min == clusters[h].u) {
                summary->del(h);
            }
            if(x == max) {
                i = summary->max;
                if(i == summary->u) {
                    max = min;
                } else {
                    max = index(i, clusters[i].max);
                }
            }
            
            return level;
        }
        
        int high(int x) {
            return floor(x/sqrt(u));
        }
        
        int low(int x) {
            return x % (int)sqrt(u);
        }
        
        int index(int h, int l) {
            return h * (int)sqrt(u) + l;
        }
};

int main() {
    
    string input;

    uint32_t s, x, y;
    int m, b, n, i, f, d, p;
    string op;
    
    cin >> s >> m >> b >> n >> i >> f >> d >> p;
    
    RNG rng = RNG(s);
    int u = pow(2, 2*m);
    vEBT t = vEBT(u);
    
    for(int j = 0; j < b; j++) {
        t.insert(rng.next() % u);
    }
    
    for(int j = 0; j < n; j++) {
        
        x = rng.next() % (i + f + d);

        if(x < i) { op = "INS"; }
        else if(x >= i && x < (i + f)) { op = "SUC"; }
        else { op = "DEL"; }
        
        if(op == "INS") {
            x = rng.next() % u;
            cout << "I " << t.insert(x) << endl;
        } else if(op == "SUC") {
            x = rng.next() % u;
            cout << "S " << t.successor(x) << endl;
        } else {
            y = rng.next() % u;
            x = t.successor(y);
            
            if(x == t.u) { x = y; }
            cout << "D " << t.del(x) << endl;
        }
    }
    
    return 0;
}