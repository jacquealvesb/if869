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

class SparseTable {
    public:
        vector<vector<uint32_t>> table;
        RNG *rng;
        int n;
        uint32_t c0;
        uint32_t (*f)(uint32_t, uint32_t);
        
        SparseTable(int n, string f, RNG *rng) {
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
            
            generate();
        }
        
        void generate() {
            generateFirstRow();
            
            uint32_t x, y;
            int m = floor(log2(n)) + 1;
            int k = 1;
            
            for(int i = 1; i < m; i++) {
                vector<uint32_t> row;

                for(int j = 0; j < n; j++) {     

                    if(j + k < table[i - 1].size()) {
                        y = table[i - 1][j + k];
                    } else {
                        y = table[i - 1].back();
                    }
                    
                    x = table[i - 1][j];
                    row.push_back(f(x, y));
                }
                table.push_back(row);
                k *= 2;
            }
        }
        
        void generateFirstRow() {
            vector<uint32_t> row;
            int m = 4*n;
            
            for(int i = 0; i < n; i++) {
                row.push_back(rng->next() % m);
            }
            
            table.push_back(row);
        }
        
        void update(int j, uint32_t v) {
            uint32_t x, y;
            int m = floor(log2(n)) + 1;
            int k = 1;
            
            table[0][j] = v;
            
            for(int i = 1; i < m; i++) {
                for(int l = j - 2*k + 1; l <= j; l++) {
                    
                    if(l + k < table[i - 1].size()) {
                        y = table[i - 1][l + k];
                    } else {
                        y = table[i - 1].back();
                    }
                    
                    x = table[i - 1][l];
                    table[i][l] = f(x, y);
                }
                k *= 2;
            }
        }
        
        uint32_t rangeQuery(int l, int r) {
            uint32_t ans = c0;
            int i;
            
            while(l < r) {
                i = log2(r - l);
                ans = f(ans, table[i][l]);
                l += pow(2, i);
            }
            
            return ans;
        }
};

int main() {
    
    string input;

    uint32_t s;
    int n, o, q, u;
    int c = 0;
    string f;
    
    while(getline(cin, input)) {
        istringstream stream(input);
        stream >> s >> n >> f >> o >> q >> u;
        
        int m = 4*n;
        RNG rng = RNG(s);
        SparseTable t = SparseTable(n, f, &rng);
        
        cout << "caso " << c << endl;
        for (int iOP = 0; iOP < o; iOP++) {
            uint32_t x = rng.next();
            
            string op = (x % (q + u)) < q ? "QRY" : "UPD";
            if(op == "QRY") {
                int l = rng.next() % n;
                int r = l + 1 + (rng.next() % (n - l));;
                
                cout << t.rangeQuery(l, r) << endl;
            } else {
                int i = rng.next() % n;
                uint32_t v = rng.next() % m;
                
                t.update(i, v);
                cout << t.rangeQuery(i, n) << endl;
            }
        }
        cout << endl;
        c++;
    }
    
    return 0;
}