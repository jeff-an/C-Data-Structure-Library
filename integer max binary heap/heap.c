#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "heap.h"

/** Array representation of a heap where the root is the first node (node 1),
/* index 2n is a right child node and index 2n + 1 is a left child node.
/* floor(log(index)) is the height of a node at a certain array index.
/* We define the height of the root as zero.
**/

struct heap {
    int count;
    int *array;
    int cap;
    struct heap **cpys;
    int cpyCount;
};

// Creates an empty heap with capacity equal to cap
Heap emptyHeap(int cap) {
    int *array = malloc(cap * sizeof(int));
    if (!array) {
        printf("Error: attempted to allocate too much capacity!");
        exit(-1);
    }
    Heap newHeap = malloc(sizeof(struct heap));
    newHeap -> count = 0;
    newHeap -> array = array;
    newHeap -> cap = cap;
    newHeap -> cpyCount = 1;
    newHeap -> cpys = malloc(100 * sizeof(Heap));
    (newHeap -> cpys)[0] = newHeap;
    return newHeap;
}

static Heap init(int *arr, int cap) {
    Heap h = malloc(sizeof(struct heap));
    h -> count = cap;
    h -> cap = cap;
    h -> array = arr;
    h -> cpyCount = 1;
    h -> cpys = malloc(100 * sizeof(Heap));
    (h -> cpys)[0] = h;
    return h;
}

Heap heapify(int *nums, int length) {
    // Assume array represents a complete binary tree
    int *array = malloc(length * sizeof(int));
    memcpy(array, nums, length * sizeof(int));

    if (length == 0 || array == NULL) {
        printf("Error: attempted to heapify an empty array");
        exit(-1);
    } else if (length < 0) {
        printf("Error: invalid array length - possible integer overflow.");
        exit(-1);
    }

    int height = floor(log(length) / log(2));
    int startingIndex = (2 << height) - 2;

    // Counter always represents a "parent" node
    for (int counter = startingIndex; counter >= 0; --counter) {
        int parent = counter;
        int left = (parent * 2) + 1;
        int right = (parent * 2) + 2;

        // Trickle down and swap until we hit the root
        while (left < length) {
            /**Debugging purposes: prints every step of the "trickle down"
            printf("counter: %d, length: %d, left: %d\n", counter, length, left);
            for (int i = 0; i < 11; printf("%d ", array[i]), ++i);
            printf("\n");
            printf("counter: %d left: %d parent: %d\n", counter, left, parent);
            **/
            if (left == length - 1) {
            // Final subtree is incomplete
                if (array[left] > array[parent]) {
                    int temp = array[parent];
                    array[parent] = array[left];
                    array[left] = temp;
                }
                break;
            }
            if (array[parent] > array[right] && array[parent] > array[left]) {
                break;
            } else if (array[right] > array[left] && array[right] > array[parent]) {
                int temp = array[parent];
                array[parent] = array[right];
                array[right] = temp;
                parent = right;
                left = (right * 2) + 1;
                right = (right * 2) + 2;
                continue;
            } else {
                int temp = array[parent];
                array[parent] = array[left];
                array[left] = temp;
                parent = left;
                right = (left * 2) + 2;
                left = (left * 2) + 1;
            }
        }
    }
    return init(array, length);
}

int insert(Heap h, int num) {
    int *array = h -> array;
    int length = h -> count;
    // Reallocate extra memory if needed
    if (h -> cap == length) {
        int *result = realloc(h -> array, 2 * sizeof(int) * h -> cap);
        if (!result) {
            printf("Error: cannot insert element, too many elements in heap!");
            return -1;
        }
        // Update cap on all shallow copies
        for (int i = 0; i < h -> cpyCount; ++i) {
            ((h -> cpys)[i]) -> cap *= 2;
        }
    }
    array[length] = num;
    int child = length;
    int parent = child % 2 == 0 ? (child - 2) / 2 : (child - 1) / 2;
    // Trickle upwards to update parent nodes if needed
    while (!(array[child] <= array[parent])) {
        if (parent == 0) {
            if (array[0] < array[child]) {
                int temp = array[0];
                array[0] = array[child];
                array[child] = temp;
                break;
            }
        } else {
            int temp = array[parent];
            array[parent] = array[child];
            array[child] = temp;
            child = parent;
            parent = parent % 2 == 0 ? (parent - 2) / 2 : (parent - 1) / 2;
        }
    }
    // Update the count on all shallow copies
    for (int i = 0; i < h -> cpyCount; ++i) {
        ++((h -> cpys)[i] -> count);
    }
    return h -> count;
}

int findMax(Heap h) {
    return (h -> array)[0];
}

int deleteMax(Heap h) {
    int *array = h -> array;
    int last = (h -> count) - 1;
    int child = last % 2 == 0 ? (last - 2) / 2 : (last - 1) / 2;
    int temp = array[child];

    // Bottom row
    if (last % 2 == 0) {
        array[child] = array[last] > array[last - 1] ? array[last] : array[last - 1];
        // If the final subtree is complete, we may need to copy the right child onto the left child
        array[last - 1] = array[array[last - 1] >= array[last] ? last : last - 1];
    } else {
        array[child] = array[last];
    }

    // Start at the right-most leaves and push the maximum upwards
    // If one child is greater than other, swap and push the larger child to the parent position
    while (1) {
        if (child == 1 || child == 2) {
            int otherChild = child % 2 == 0 ? child - 1 : child + 1;
            array[0] = temp > array[otherChild] ? temp : array[otherChild];
            array[otherChild] = array[otherChild] >= temp ? temp : array[otherChild];
            break;
        }
        int otherChild = child % 2 == 0 ? child - 1 : child + 1;
        int parent = child % 2 == 0 ? (child - 2) / 2 : (child - 1) / 2;
        int toBeTemp = array[parent];
        array[parent] = temp > array[otherChild] ? temp : array[otherChild];
        array[otherChild] = array[otherChild] >= temp ? temp : array[otherChild];
        temp = toBeTemp;
        child = parent;
    }
    // Update count on all shallow copies
    for (int i = 0; i < h -> cpyCount; ++i) {
        --((h -> cpys)[i] -> count);
    }
    return h -> count;
}

// WARNING: Shallow copies of heaps autoupdate cap and length
Heap shallowCopy(Heap h) {
    if (h -> cpyCount >= 100) {
        printf("Error: too many shallow copies created!");
        exit(-1);
    }

    Heap newHeap = malloc(sizeof(struct heap));
    newHeap -> cap = h -> cap;
    newHeap -> count = h -> count;
    newHeap -> array = h -> array;
    newHeap -> cpyCount = ++(h -> cpyCount);
    newHeap -> cpys = h -> cpys;
    (h -> cpys)[h -> cpyCount - 1] = newHeap;

    return newHeap;
}

Heap deepCopy(Heap h) {
    Heap newHeap = malloc(sizeof(struct heap));
    int *array = malloc((h -> cap) * sizeof(int));
    memcpy(array, h -> array, (h -> cap) * sizeof(int));
    newHeap -> count = h -> count;
    newHeap -> cap = h -> cap;
    newHeap -> array = array;
    return newHeap;
}

// Utility Functions
void printHeap(Heap h) {
    if (h -> count == 0) {
        printf("Heap is empty!");
        return;
    }
    int *arr = h -> array;
    int count = h -> count;
    int height = floor(log(count) / log(2));
    int totalSpaces = 2 << (height + 1) - 1;
    if (totalSpaces < 0) {
        printf("Too many elements in the heap to print (max height is 30)!");
        return;
    }
    for (int level = 0; level <= height; ++level) {
        int startIndex = level == 0 ? 0 : pow(2, level) - 1;
        int endDiv = pow(2, (height - level)) - 1;
        int midDiv = level == 0 ? 0 : (totalSpaces - 2 * endDiv - pow(2, level)) / (pow(2, level) - 1);
        for (int i = 0; i < endDiv; printf(" "), ++i);
        printf("%d", arr[startIndex]);
        for (int j = 1; j < pow(2, level) && (startIndex + j + 1) <= count; ++j) {
            for (int z = 0; z < midDiv; printf(" "), ++z);
            printf("%d", arr[startIndex + j]);
        }
        for (int i = 0; i < endDiv; printf(" "), ++i);
        printf("\n");
        printf("\n");
    }
    return;
}

int getHeight(Heap h) {
    return floor(log(h -> count) / log(2)) < 0 ? -1 : floor(log(h -> count) / log(2));
}

int getCount(Heap h) {
    return h -> count;
}

int getCapacity(Heap h) {
    return h -> cap;
}

/** Driver program for debugging/demo purposes
int main() {
    // Heapify test
    int array[] = {4, 2, 1, 5, 20, -1, 0, 9, 7, 8, 10, 30, 40};
    Heap h = heapify(array, sizeof(array) / sizeof(int));
    printHeap(h);
    printHeap(h);

    // Insert and delete test
    Heap h1 = emptyHeap(100);
    insert(h1, -50);
    insert(h1, 5);
    insert(h1, 4);
    insert(h1, 70);
    insert(h1, 100);
    insert(h1, 0);
    insert(h1, 2);
    insert(h1, 99);
    printHeap(h1);
    deleteMax(h1);
    printHeap(h1);

    // Copy test
    Heap h2 = shallowCopy(h1);
    Heap h3 = deepCopy(h1);
    deleteMax(h1);
    printHeap(h2);
    printHeap(h1);
    printHeap(h3);

    return 0;
} **/
