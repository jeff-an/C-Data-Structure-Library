//struct heap;
typedef struct heap *Heap;
Heap emptyHeap(int cap);
Heap heapify(int *array, int length);
int getCount(Heap h);
int getCapacity(Heap h);
static Heap init(int *arr, int cap);
int getHeight(Heap h);
void printHeap(Heap h);