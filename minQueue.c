#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>

// -------------- DynamicArray -------------- //

struct DynamicArray {
    uint32_t *values;
    int start, size, cap;
};

struct DynamicArray *newDynamicArray() {
    struct DynamicArray *a = (struct DynamicArray *)malloc(sizeof(struct DynamicArray));
    a->values = (uint32_t *)calloc(1, sizeof(uint32_t));
    a->start, a->size = 0;
    a->cap = 1;
    return a;
}

void updateCapacity(struct DynamicArray *a, int newCap) {
    a->cap = newCap;

    uint32_t *tmp = a->values;
    int old = a->start;

    a->values = (uint32_t *)calloc(newCap, sizeof(uint32_t));
    for(int i = 0; i < a->size; i++) {
        if((old + i) > (a->cap)) {
            old = 0;
        }
        a->values[i] = tmp[old];
        old++;
    }
    a->start = 0;
    free(tmp);
}

int fixedIndex(int i, struct DynamicArray *a) {
    int newIndex = a->start + i;

    if(newIndex >= a->cap) {
        return newIndex - a->cap;
    } else {
        return newIndex;
    }
}

void insert(int val, struct DynamicArray *a) {
    if((float)(a->size)/(float)(a->cap) >= 0.5) {
        updateCapacity(a, a->cap * 2);
    }

    int index = fixedIndex(a->size, a);
    a->values[index] = val;
    a->size = a->size + 1;
}

uint32_t deleteLast(struct DynamicArray *a) {
    if((float)(a->size)/(float)(a->cap) <= 0.25) {
        updateCapacity(a, a->cap / 2);
    }

    int index = fixedIndex(a->size - 1, a);
    uint32_t val = a->values[index];
    a->size = a->size - 1;
    return val;
}

uint32_t deleteFirst(struct DynamicArray *a) {
    if((float)(a->size)/(float)(a->cap) <= 0.25) {
        updateCapacity(a, a->cap / 2);
    }

    uint32_t val = a->values[a->start];
    a->size = a->size - 1;
    a->start = (a->start + 1);
    if(a->start >= a->cap) {
        a->start = 0;
    }

    return val;
}

// -------------- Queue -------------- //

struct Queue {
    struct DynamicArray *V;
    struct DynamicArray *I;
};

struct Queue *newQueue() {
    struct Queue *q = (struct Queue *)malloc(sizeof(struct Queue));
    q->V = newDynamicArray();
    q->I = newDynamicArray();
    return q;
}

uint32_t min(struct Queue *q) {
    int firstIndex = q->I->values[q->I->start];
    int index = fixedIndex(firstIndex, q->V);
    return q->V->values[index];
}

void enqueue(int val, struct Queue *q) {
    insert(val, q->V);
    
    int lastIndex = q->I->values[q->I->size - 1];
    int index = fixedIndex(lastIndex, q->V);
    while(q->I->size > 0 && q->V->values[index] > val) {
        deleteLast(q->I);
        lastIndex = q->I->values[q->I->size - 1];
        index = fixedIndex(lastIndex, q->V);
    }
    
    insert(q->V->size - 1, q->I);
}

uint32_t dequeue(struct Queue *q) {
    int val = deleteFirst(q->V);
    
    for(int i = 0; i < q->I->size; i++) {
        q->I->values[i] = q->I->values[i] - 1;
    }

    int firstIndex = q->I->values[q->I->start];
    int index = fixedIndex(firstIndex, q->V);
    if(q->V->values[index] == val) {
        deleteFirst(q->I);
    } 
    
    return val;
}

// -------------- Program -------------- //

uint32_t generateNumber(uint32_t seed) {
    uint32_t A = 1664525;
    uint32_t C = 1013904223;
    uint32_t R = (uint32_t)pow(2, 32);
    return (A * seed + C) % R;
}

int main() {
    struct Queue *queue = newQueue();
    
    uint32_t s;
    int b, n, p;
    scanf("%" SCNd32 , &s);
    scanf("%d %d %d", &b, &n, &p);

    for(int i = 0; i < b; i++) {
        enqueue(s, queue);
        s = generateNumber(s);
    }

    for(int i = 0; i < n; i++) {
        if(s % 10 < p) {
            s = generateNumber(s);
            enqueue(s, queue);
        } else {
            dequeue(queue);
        }
        
        printf("%d %d\n", queue->I->size, queue->I->values[queue->I->start]);
        s = generateNumber(s);
    }

    return 0;
}