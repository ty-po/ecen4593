#include <iostream>

struct Node {
  unsigned long long int tag;
  unsigned long long int address;
  bool valid;
  bool dirty;
  Node * next;
  Node * previous;
};

class LRU {
  public:
    Node * head;
    Node * tail;
    
    unsigned int size;

    LRU(unsigned int maxSize);
    Node * getNode(unsigned int index);
    void toFront(Node * current);

    Node * contains(unsigned long long int tag);

    void markDirty();
};


class Cache {
  public:
    LRU * victimCache;
    
    LRU ** indexArray;

    unsigned int cacheSets;
    unsigned int ways;

    unsigned int blockOffsetBits;
    unsigned int indexBits;
    unsigned int tagBits;

    Cache(unsigned int Size, unsigned int Ways, unsigned int BlockSize, unsigned int vcSize, unsigned int addressBits);

    unsigned long int getIndex(unsigned long long int address);
    unsigned long long int getTag(unsigned long long int address);

    Node * contains(unsigned long long int address);
    
    void markDirty(unsigned long long int address);
    /******
     0 = not in cache
     1 = in victim cache
     2 = in cache
    ******/
};
