struct Node {
    void *data;
    struct Node *next;
};
void cons(void *data, struct Node **lst);
int length(struct Node **lst);
void pop(struct Node **lst);
int popInt(struct Node **lst);
char *popStr(struct Node **lst);
int peekInt(struct Node **lst);
char *peekStr(struct Node **lst);
void freeList(struct Node **lst);
void freeListImpl(struct Node *lst);
