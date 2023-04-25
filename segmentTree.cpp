#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <vector>
#include <limits>

using namespace std;

uint32_t min(uint32_t x, uint32_t y) { return x < y ? x : y; }
uint32_t max(uint32_t x, uint32_t y) { return x > y ? x : y; }
uint32_t sum(uint32_t x, uint32_t y) { return x + y; }

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

class SegmentTree {
    public:
        vector<uint32_t> t;
        RNG *rng;
        int n;
        uint32_t c0;
        uint32_t (*f)(uint32_t, uint32_t);
        
        SegmentTree(int n, string f, RNG *rng) {
            this->n = n;
            this->rng = rng;
            
            if(f == "MIN") { 
                this->f = min; 
                this->c0 = std::numeric_limits<uint32_t>::max();
            } else if(f == "MAX") { 
                this->f = max; 
                this->c0 = std::numeric_limits<uint32_t>::min();; 
            } else if(f == "SUM") { 
                this->f = sum; 
                this->c0 = 0; 
            }
            
            build(n);
        }

        void build(int n) {
            int m = 4*n;
            t = vector<uint32_t>(2*n);

            for(int i = 0; i < n; i++) {
                t[i + n] = rng->next() % m;
            }
            for(int i = n-1; i >= 0; i--) {
                t[i] = f(t[2*i], t[2*i + 1]);
            }
        }
        
        void update(int j, uint32_t v) {
            int i = n + j;
            
            t[i] = v;
            i = floor(i/2);
            while(i >= 1) {
                t[i] = f(t[2*i], t[2*i + 1]);
                i = floor(i/2);
            }
        }
        
        uint32_t rangeQuery(int l, int r) {
            int nl = n + l;
            int nr = n + r;
            uint32_t ans = c0;

            while(nl < nr) {
                if(nl % 2 != 0) {
                    ans = f(ans, t[nl]);
                    nl = nl + 1;
                }
                if(nr % 2 != 0) {
                    nr = nr - 1;
                    ans = f(ans, t[nr]);
                }
                nl = floor(nl/2);
                nr = floor(nr/2);
            }
            return ans;
        }
};

int main() {
    cin.tie(0);ios_base::sync_with_stdio(0);
    string input;

    uint32_t s, res;
    int n, w, q, u, p, c = 0;
    string f;
    
    while(getline(cin, input)) {
        istringstream stream(input);
        stream >> s >> n >> f >> w >> q >> u >> p;
        
        int m = 4*n;
        RNG rng = RNG(s);
        SegmentTree t = SegmentTree(n, f, &rng);
        
        cout << "caso " << c << "\n";
        for (int j = 0; j < w; j++) {
            uint32_t x = rng.next();
            
            string op = (x % (q + u)) < q ? "QRY" : "UPD";
            if(op == "QRY") {
                int l = rng.next() % n;
                int r = l + 1 + (rng.next() % (n - l));;
                
                res = t.rangeQuery(l, r);
            } else {
                int i = rng.next() % n;
                uint32_t v = rng.next() % m;
                
                t.update(i, v);
                res = t.rangeQuery(i, n);
            }

            if(j % p == 0) {
                cout << to_string(res) << "\n";
            }
        }
        cout << "\n";
        c++;
    }
    
    return 0;
}