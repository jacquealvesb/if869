#include <iostream>
#include <math.h>
#include <vector>
#include <limits>

using namespace std;

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

class Node {
    public:
        int height;
        uint32_t key, val;
        vector<Node *> next;

        Node(int height) {
            this->key = numeric_limits<uint32_t>::min();
            this->height = height;
            this->next = vector<Node *>(height);
        }
};

class SkipList {
    public:
        RNG *rng;
        int height = 1;
        Node *head;
        
        SkipList(RNG *rng) {
            this->rng = rng;
            this->head = new Node(height);
        }
        
        vector<Node *> precursors(uint32_t key, int *nodes) {
            vector<Node *> p = vector<Node *>(height);
            Node *cur = head;
            *nodes = 1;
            
            for(int l = height - 1; l >= 0; l--) {
                while(cur->next[l] != NULL && cur->next[l]->key < key) {
                    cur = cur->next[l];
                    *nodes += 1;
                }
                p[l] = cur;
            }
            return p;
        }
        
        int insert(uint32_t key) {
            int nodes = 0;
            vector<Node *> p = precursors(key, &nodes);
            
            if(p[0]->next[0] != NULL && p[0]->next[0]->key == key) {
                return 0;
            }
            
            int h = randomHeight();
            if(h > height) {
                
                for(int i = height; i < h; i++) {
                    head->next.push_back(NULL);
                }

                for(int i = height; i < h; i++) {
                    p.push_back(head);
                }
                
                height = h;
            }
            
            Node *n = new Node(h);
            n->key = key;
            n->val = key;
            
            for(int i = 0; i < h; i++) {
                n->next[i] = p[i]->next[i];
                p[i]->next[i] = n;
                
            }
            
            return 1;
        }
        
        int del(uint32_t key) {
            int nodes = 0;
            vector<Node *> p = precursors(key, &nodes);
            
            if(p[0]->next[0] == NULL || p[0]->next[0]->key != key) {
                return 0;
            }
            
            Node *n = p[0]->next[0];
            
            for(int i = 0; i < n->height; i++) {
                p[i]->next[i] = n->next[i];
            }
            
            while(height > 1 && head->next[height - 1] == NULL) {
                height -= 1;
            }
            
            return 1;
        }

        int search(uint32_t key, int *nodes) {
            vector<Node *> p = precursors(key, nodes);
            
            if(p[0]->next[0] != NULL && p[0]->next[0]->key == key) {
                return p[0]->next[0]->height;
            } else {
                return 0;
            }
        }
        
        int randomHeight() {
            int h = 1;
            int maxH = height + 1;
            
            while(rng->next() % 100 < 50 && h < maxH) {
                h += 1;
            }
            return h;
        }
};

int main()
{
    uint32_t s, x;
    int u, b, n, f, i, d, p, res;
    string op, out;
    
    cin >> s >> u >> b >> n >> f >> i >> d >> p;

    RNG rng = RNG(s);
    SkipList l = SkipList(&rng);
    
    for(int j = 0; j < b; j++) {
        l.insert(rng.next() % u);
    }
    
    for(int j = 0; j < n; j++) {
        
        x = rng.next() % (f + i + d);
        
        if(x < f) { op = "FND"; }
        else if(x >= f && x < (f + i)) { op = "INS"; }
        else { op = "DEL"; }
    
        x = rng.next() % u;
        
        if(op == "FND") {
            int nodes = 0;
            int height = l.search(x, &nodes);
            out = "F " + to_string(nodes) + " " + to_string(height);
        } else if(op == "INS") {
            res = l.insert(x);
            out = "I " + to_string(res);
        } else {
            res = l.del(x);
            out = "D " + to_string(res);
        }

        if(j % p == 0) {
            cout << out << "\n";
        }
        
    }
    
    return 0;
}