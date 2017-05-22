#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

void cons(void *data, struct Node **lst) {
    struct Node *car = malloc(sizeof(struct Node));
    car -> data = data;
    car -> next = *lst;
    *lst = car;
    return;
}

int peekInt(struct Node **lst) {
    int *data = (*lst) -> data;
    return *data;
}

char *peekStr(struct Node **lst) {
    char *data = (*lst) -> data;
    return data;
}

int length(struct Node **lst) {
    int sofar = 0;
    struct Node *pointer = *lst;
    while (pointer) {
        pointer = pointer -> next;
        ++sofar;
    }
    return sofar;
}

void pop(struct Node **lst) {
    struct Node *newRoot = (*lst) -> next;
    free((*lst) -> data);
    free(*lst);
    *lst = newRoot;
    return;
}

int popInt(struct Node **lst) {
    struct Node *newRoot = (*lst) -> next;
    int *data = (*lst) -> data;
    int out = *data;
    free(*lst);
    *lst = newRoot;
    return out;
}

char *popStr(struct Node **lst) {
    struct Node *newRoot = (*lst) -> next;
    char *data = (*lst) -> data;
    free(*lst);
    *lst = newRoot;
    return data;
}

void freeList(struct Node **lst) {
    freeListImpl(*lst);
    return;
}

void freeListImpl(struct Node *lst) {
    if (lst == NULL) return;
    freeListImpl(lst -> next);
    free(lst -> data);
    free(lst);
    return;
}
