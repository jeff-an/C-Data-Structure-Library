struct SeqImpl;
typedef struct SeqImpl *Sequence;

Sequence emptySeq();
int seqSize(Sequence s);
void add(Sequence s, int val);
void removeIndex(Sequence s, int element);
int index(Sequence s, int element);
void freeSeq(Sequence s);
void printSeq(Sequence s);
