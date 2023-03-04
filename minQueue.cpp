#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

class Queue {       
  public:             
    vector<uint32_t> V;      
    vector<uint32_t> I;
    
    uint32_t min() {
        return V.at(I.front());
    }
    
    void enqueue(int val) {  
        V.push_back(val);
        
        while(!I.empty() && V.at(I.back()) > val) {
            I.pop_back();
        }
        
        I.push_back(V.size() - 1);
    }
    
    void dequeue() {
        uint32_t currentMin = min();
        uint32_t dequeued = V.front();
        
        V.erase(V.begin());
        for (auto & val : I) {
            val = val - 1;
        }
        
        if(currentMin == dequeued) {
            I.erase(I.begin());
        }
    }
};

uint32_t generateNumber(uint32_t seed) {
    uint32_t A = 1664525;
    uint32_t C = 1013904223;
    uint32_t R = (uint32_t)pow(2, 32);
    return (A * seed + C) % R;
}

int main() {
    
    Queue q;
    
    uint32_t s;
    int b, n, p;
    cin >> s >> b >> n >> p;

    for(int i = 0; i < b; i++) {
        q.enqueue(s);
        s = generateNumber(s);
    }

    for(int i = 0; i < n; i++) {
        if(s % 10 < p) {
            s = generateNumber(s);
            q.enqueue(s);
        } else {
            q.dequeue();
        }
        
        cout << q.I.size() << " " << q.I.front() << "\n";
        s = generateNumber(s);
    }
    
  return 0;
}