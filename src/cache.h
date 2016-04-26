#include <iostream>
#include <iomanip>
#include <string>

struct Node {
  unsigned int index;
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
    unsigned int used;

    LRU(unsigned int maxSize);
    Node * getNode(unsigned int index);
    void toFront(Node * current);
    void toBack(Node * current);

    Node * contains(unsigned long long int tag);

    bool isFull();

    bool push(Node * current);

    void remove(Node * current);

    void printLRU();
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

    unsigned int getIndex(unsigned long long int address);
    unsigned long long int getTag(unsigned long long int address);

    Node * contains(unsigned long long int address);

    void toFront(Node * current);

    void cleanHead();

    bool push(Node * current);

    void printCache(std::string cacheName, unsigned int vcSize);
};
