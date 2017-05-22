#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include <string.h>

static const int start = 2;

struct Node {
    char *str;
    int data;
    int hashCode; // Index that this string has been hashed to
    struct Node *next;
};

struct hashImpl {
    int size, cap;
    node *array;
    node lst;
};

static unsigned long hash(unsigned char *str) {
    int counter = 0;
    unsigned long hash = 5381;
    int c;
    while (c = *(str++) && counter < 50) {
        hash = (hash * 33) ^ c;
    }
    return hash;
}

static void rehash(Hash h) {
    if ((float)h -> size / h -> cap < 1) return;
    int newCap = 2 * h -> cap;
    node *newArray = malloc(newCap * sizeof(node));
    memset(newArray, 0, newCap * sizeof(node));
    for (node cursor = h -> lst; cursor; cursor = cursor -> next) {
        int hashCode = hash(cursor -> str) % newCap;
        freeList(newArray[cursor -> hashCode]);
        cursor -> hashCode = hashCode;
        newArray[hashCode] = cons(cursor -> str, cursor -> data, hashCode, newArray[hashCode]);
    }
    free(h -> array);
    h -> array = newArray;
    h -> cap *= 2;
    return;
}

static node cons(char *str, int data, int hashCode, node cdr) {
    node newNode = malloc(sizeof(struct Node));
    newNode -> str = str;
    newNode -> data = data;
    newNode -> hashCode = hashCode;
    newNode -> next = cdr;
    return newNode;
}

Hash newHash() {
    Hash h = malloc(sizeof(struct hashImpl));
    h -> size = 0, h -> cap = start, h -> lst = NULL, h -> array = malloc(start * sizeof(node));
    return h;
}

void add(Hash h, char *str, int data) {
	rehash(h);
    int hashCode = hash(str) % h -> cap;
	h -> lst = cons(str, data, hashCode, h -> lst);
	(h -> array)[hashCode] = cons(str, data, hashCode, (h -> array)[hashCode]);
	++h -> size;
	return;
}

int retrieve(Hash h, char *str) {
	node cursor = (h -> array)[hash(str) % h -> cap];
	while (cursor) {
		if (strcmp(str, cursor -> str) == 0) { return cursor -> data; }
		cursor = cursor -> next;
	}
	return -1;
}

void static freeList(node ptr) {
    if (ptr == NULL) return;
    else freeList(ptr -> next);
    free(ptr);
    ptr = NULL;
    return;
}

void freeHash(Hash h) {
    node ptr = h -> lst;
    while (ptr != NULL) {
        freeList((h -> array)[ptr -> hashCode]);
        h -> array = NULL;
        ptr = ptr -> next;
    }
    freeList(h -> lst);
    free(h -> array);
    h -> size = 0;
    h -> cap  = 0;
    free(h);
}
