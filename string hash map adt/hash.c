#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include <string.h>

static const int start = 100;
static const float loadFactor = 0.75;

/** Definitions **/

struct Node {
    char *str;
    int data;
    int hashCode; // Index that this string has been hashed to
    struct Node *next;
};

struct hashImpl {
    int size, cap;
    node *array;
    node lst; // Used for freeing memory and reallocating memory
};

/** Main functions **/

Hash newHash() {
    Hash h = malloc(sizeof(struct hashImpl));
    h -> size = 0, h -> cap = start, h -> lst = NULL, h -> array = malloc(start * sizeof(node));
    memset(h -> array, 0, start * sizeof(node));
    return h;
}


void add(Hash h, char *key, int data) {
// Original string is not modified, copy is created
	rehash(h);
    char *str = malloc(100 * sizeof(char));
    str = strcpy(str, key);
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
	return 2 << 30;
}

static void freeList(node ptr) {
    if (ptr == NULL) return;
    else freeList(ptr -> next);
    free(ptr -> str);
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
    h -> size = 0; // Safety
    h -> cap  = 0; // Safety
    free(h);
}

void delete(Hash h, char *str) {
// Deletes all copies of mappings where the key is equal to str
// The linked list is not updated - consolidation occurs lazily
    node lst = (h -> array)[hash(str) % h -> cap];
    (h -> array)[hash(str) % h -> cap] = deleteImpl(lst, str);
    return;
}

static node deleteImpl(node lst, char *str) {
    node root = NULL;
    node toSet = NULL;
    node previousValid = NULL;
    while (lst) {
        if (strcmp(str, lst -> str)) { // Not a match
            if (root == NULL) {
                root = lst;
            }
            if (toSet != NULL) {
                toSet -> next = lst;
                toSet = NULL;
            }
            previousValid = lst;
            lst = lst -> next;
        } else {
            toSet = previousValid;
            node temp = lst;
            lst = lst -> next;
            free(temp);
        }
    }
    if (toSet != NULL) {
        toSet -> next = NULL;
    }
    return root;
}

Hash hashCopy(Hash h) {
// Returns a fresh deep copy of Hash h
// Consolidates the template hash map as well
    node cursor = h -> lst;
    Hash new = newHash();
    node previousValid = NULL;
    node root = NULL;
    int setFlag = 0;
    while (cursor) {
        char *key = cursor -> str;
        int data = cursor -> data;
        if (retrieve(h, key) != 2 << 30) {
            add(new, key, data); // Add to new hash map
            delete(h, key);
            if (root == NULL) {
                root = cursor;
            }
            if (setFlag) { // Coming from a delete operation
                previousValid -> next = cursor;
                setFlag = 0;
            }
            previousValid = cursor;
            cursor = cursor -> next;
        } else {
            setFlag = 1;
            node temp = cursor;
            cursor = cursor -> next;
            free(temp -> str);
            free(temp);
        }
    }
    h -> lst = root;
    if (setFlag) {
        previousValid -> next = NULL;
    }
    return new;
}

/** Utility and Auxiliary Functions **/

int getSize(Hash h) {
    return h -> size;
}

int getCap(Hash h) {
    return h -> cap;
}

char *getLastInsertedKey(Hash h) {
    return (h -> lst) -> str;
}

unsigned long hash(unsigned char *str) {
    int counter = 0;
    unsigned long hash = 5381;
    int c;
    while (c = *(str++) && counter < 50) {
        hash = (hash * 33) ^ c;
    }
    return hash;
}

static void rehash(Hash h) {
    if ((float)h -> size / h -> cap < loadFactor) return;
    node *array = h -> array;
    int newCap = 2 * h -> cap;
    node *newArray = malloc(newCap * sizeof(node));
    if (!newArray) {
        printf("Error: too many nodes added, insufficient memory!");
        exit(-1);
    }
    memset(newArray, 0, newCap * sizeof(node));
    node previous = NULL;
    node cursor = h -> lst;
    while (cursor) {
        int newHashCode = hash(cursor -> str) % newCap; // New hash code
        if (retrieve(h, cursor -> str) != 2 << 30) { // If the key still exists
            cursor -> hashCode = newHashCode;
            newArray[newHashCode] = cons(cursor -> str, cursor -> data, newHashCode, newArray[newHashCode]);
            delete(h, cursor -> str);
            previous = cursor;
            cursor = cursor -> next;
        } else { // Delete the node entry from the linked list
            node next = cursor -> next;
            free(cursor);
            if (previous != NULL) {
                previous -> next = next;
            } else {
                h -> lst = next;
            }
            cursor = next;
        }
    }
    for (node cursor = h -> lst; cursor; cursor = cursor -> next) {
        freeList(array[hash(cursor -> str) % h -> cap]);
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

/** Driver to test functions
int main() {
    Hash test = newHash(10);
    add(test, "hello", 1);
    printf("should read 1: %d\n", retrieve(test, "hello"));
    add(test, "hello", 2);
    printf("should read 2: %d\n", retrieve(test, "hello"));
    add(test, "goodbye", 3);
    add(test, "world", 4);
    add(test, "trump", 5);
    printf("should read 3: %d\nshould read 4: %d\nshould read 5: %d\n", retrieve(test, "goodbye"), retrieve(test, "world"), retrieve(test, "trump"));
    printf("should read 2: %d\n", retrieve(test, "hello"));
    delete(test, "hello");
    printf("should be very negative: %d\n", retrieve(test, "hello"));
    return 0;
} **/
