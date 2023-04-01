#include <iostream>
#include <string>
#include <math.h>
#include <vector>

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
        Node *left, *right, *par = NULL;
        uint32_t key = 0;
};

class SplayTree {
    public:
        Node *head;

    void insert(uint32_t key) {
        Node *newNode = Node(key);
        Node *temp = head;
        Node *p;

        while(temp != NULL) {
            p = temp;
            if(key < temp->key) {
                temp = temp->left;
            } else {
                temp = temp->right;
            }
        }
        newNode->par = p;
        if(p == NULL) {
            head = newNode;
        } else if(newNode->key < p->key) {
            y->left = newNode;
        } else {
            y->right = newNode;
        }
        splay(newNode);
    }
};

Node *rotateLeft(Node *x) {
    Node *p = x->par;
    Node *r = x->right;
    Node *rl = r->left;

    x->right = rl;
    if(rl) {
        rl->par = x;
    }
    r->left = x;
    x->par = r;
    r->par = p;
    if(p && x == p->left) {
        p->left = r;
    } else if(p && x == p->right) {
        p->right = r;
    }
    return r;
}

Node *rotateRight(Node *x) {
    Node *p = x->par;
    Node *l = x->left;
    Node *lr = l->right;

    x->left = lr;
    if(lr) {
        lr->par = x;
    }
    l->right = x;
    x->par = r;
    l->par = p;
    if(p && x == p->right) {
        p->right = l;
    } else if(p && x == p->left) {
        p->left = l;
    }
    return l;
}

Node *zig(Node *x) {
    Node *p = x->par;
    if(x == p->left) {
        return rotateRight(p);
    } else {
        return rotateLeft(p);
    }
}

Node *zigzag(Node *x) {
    Node *p = x->par;
    Node *g->par;

    if(p == g->left) {
        if(x == p->left) {
            rotateRight(g);
            return rotateRight(p);
        } else {
            rotateLeft(p);
            return rotateRight(g);
        }
    } else {
        if(x == p->right) {
            rotateLeft(g);
            return rotateLeft(p);
        } else {
            rotateRight(p);
            return rotateLeft(g);
        }
    }
}

Node *splay(Node *x) {
    while(x->par != NULL) {
        Node *g = x->par->par;
        if(g == NULL) {
            zig(x);
        } else {
            zigzag(x);
        }
    }
    return x;
}

int main() {

    uint32_t s;
    int u;
    
    cin >> s >> u;
    
    RNG rng = RNG(s);
    SplayTree t = SplayTree(rng.next() % u);
    
    return 0;
}