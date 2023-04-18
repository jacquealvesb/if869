#include <iostream>
#include <math.h>

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

struct Node {
    public:
        uint32_t key;
        Node *par, *left, *right;

        Node(uint32_t k) {
            key = k;
            par = left = right = NULL;
        }
};

class SplayTree {
    public:
        Node* root;

        void rotateLeft(Node* x) {
            Node* r = x->right;

            x->right = r->left;
            if (r->left != NULL) {
                r->left->par = x;
            }

            r->par = x->par;
            if (x->par == NULL) {
                root = r;
            } else if (x == x->par->left) {
                x->par->left = r;
            } else {
                x->par->right = r;
            }

            r->left = x;
            x->par = r;
        }

        void rotateRight(Node* x) {
            Node* l = x->left;

            x->left = l->right;
            if (l->right != NULL) {
                l->right->par = x;
            }

            l->par = x->par;
            if (x->par == NULL) {
                root = l;
            } else if (x == x->par->left) {
                x->par->left = l;
            } else {
                x->par->right = l;
            }

            l->right = x;
            x->par = l;
        }

        void zig(Node *x) {
            Node *p = x->par;
            if(x == p->left) {
                rotateRight(p);
            } else {
                rotateLeft(p);
            }
        }

        void zigzag(Node *x) {
            Node *p = x->par;
            Node *g = p->par;

            if(p == g->left) {
                if(x == p->left) {
                    rotateRight(g);
                    rotateRight(p);
                } else {
                    rotateLeft(p);
                    rotateRight(g);
                }
            } else {
                if(x == p->right) {
                    rotateLeft(g);
                    rotateLeft(p);
                } else {
                    rotateRight(p);
                    rotateLeft(g);
                }
            }
        }

        Node* splay(Node* x) {
            Node *g;
            while (x->par != NULL) {
                g = x->par->par;
                if(g == NULL) {
                    zig(x);
                } else {
                    zigzag(x);
                }
            }
            return x;
        }

        int query(uint32_t key) {
            Node* current = root;
            int depth = 0;

            while (current != NULL) {
                if (key == current->key) {
                    root = splay(current);
                    return depth;
                } else if (key < current->key) {
                    current = current->left;
                } else {
                    current = current->right;
                }
                depth++;
            }
            return -1;
        }

        int insert(uint32_t key) {
            Node* current = root;
            Node* par = NULL;
            int depth = 0;

            while (current != NULL) {
                par = current;
                if (key < current->key) {
                    current = current->left;
                } else if (key > current->key) {
                    current = current->right;
                } else {
                    return -1;
                }
                depth++;
            }

            Node* newNode = new Node(key);
            newNode->par = par;
            if (par == NULL) {
                root = newNode;
                return 0;
            } else if (key < par->key) {
                par->left = newNode;
            } else         {
                par->right = newNode;
            }
            return depth;
        }
};

int main()
{
    uint32_t s, x;
    int u, b, n, i, q, p, d;
    string op;
    
    cin >> s >> u >> b >> n >> i >> q >> p;
    
    RNG rng = RNG(s);
    SplayTree t = SplayTree();

    for(int j = 0; j < b; j++) {
        t.insert(rng.next() % u);
    }
    
    for(int j = 0; j < n; j++) {
        
        x = rng.next();
        
        op = (x % (i + q) < i) ? "INS" : "QRY";
        x = rng.next() % u;
        
        if(op == "INS") {
            d = t.insert(x);
        } else {
            d = t.query(x);
        }

        if(j % p == 0) {
            cout << op[0] << " " << x << " " << d << endl;
        }
    }
    
    return 0;
}