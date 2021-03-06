struct hashImpl;
typedef struct hashImpl *Hash;
void freeHash(Hash h);
struct Node;
typedef struct Node *node;
Hash newHash();
void static freeList(node ptr);
static node cons(char *str, int data, int hashCode, node cdr);
void add(Hash h, char *str, int data);
int retrieve(Hash h, char *str);
void delete(Hash h, char *str);
static node deleteImpl(node lst, char *str);
int getSize(Hash h);
int getCap(Hash h);
char *getLastInsertedKey(Hash h);
unsigned long hash(unsigned char *str);
static void rehash(Hash h);
Hash hashCopy(Hash h);
