#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <vector>

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
        int n;
        uint32_t (*f)(uint32_t, uint32_t);
        RNG *rng;
        vector<vector<uint32_t>> table;
        
        SparseTable(int n, string f, RNG *rng) {
            this->n = n;
            this->rng = rng;
            
            if(f == "MIN") { this->f = min; }
            else if(f == "MAX") { this->f = max; }
            else if(f == "SUM"){ this->f = sum; }
            
            generate();
        }
        
        void generate() {
            generateFirstRow();
            
            int m = floor(log(n)) + 1;
            int k = 1;
            for(int i = 1; i < m; i++) {
                vector<uint32_t> row;
                for(int j = 0; j < n; j++) {
                    uint32_t x = table[i - 1][j];
                    uint32_t y = table[i - 1][j + k];
                    row.push_back(f(x, y));
                }
                table.push_back(row);
                k = 2*k;
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
};

int main() {
    
    string input;

    uint32_t s;
    int n, o, q, u;
    string f;
    
    while(getline(cin, input)) {
        istringstream stream(input);
        stream >> s >> n >> f >> o >> q >> u;
        
        RNG rng = RNG(s);
        SparseTable st = SparseTable(n, f, &rng);
        
        for(int i = 0; i < st.table.size(); i++) {
            for(int j = 0; j < st.table[i].size(); j++) {
                cout << st.table[i].at(j) << " - ";
            }
            cout << endl;
        }

        cout << endl;
    }
    
  return 0;
}