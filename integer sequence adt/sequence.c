#include <stdlib.h>
#include <stdio.h>
#include "sequence.h"

struct SeqImpl {
    int size, *array, limit;
};

const int global_limit = 10;

void printSeq(Sequence s) {
    for (int i = 0; i < s -> size; printf("%d ", (s -> array)[i++]));
    return;
}

Sequence emptySeq() {
    Sequence newSeq = malloc(sizeof(struct SeqImpl));
    newSeq -> size = 0;
    newSeq -> array = malloc(sizeof(int) * global_limit);
    newSeq -> limit = global_limit;
    return newSeq;
}

int seqSize(Sequence seq) {
    return seq -> size;
}

void add(Sequence seq, int val) {
    if (seq -> size == seq -> limit) {
        int *newArray = realloc(seq -> array, 2 * seq -> limit * sizeof(int));
        seq -> array = newArray;
        newArray[seq -> size++] = val;
        seq -> limit *= 2;
        return;
    }

    else {
        (seq -> array)[(seq->size)++] = val;
        return;
    }
}

void removeIndex(Sequence seq, int element) {
    if (element > seq -> size || element <= 0) return;
    for (int i = element; i < seq -> size; (seq -> array)[i] = (seq -> array)[++i]);
    --(seq -> size);
    return;
}

int index(Sequence seq, int element) {
    return (seq -> array)[element];
}

void freeSeq(Sequence seq) {
    free(seq -> array);
    seq -> size = 0;
    seq -> array = seq -> size = seq -> limit = 0;
    return;
}
