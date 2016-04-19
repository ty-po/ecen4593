struct Node {
  unsigned long long int address;
  bool valid;
  bool dirty;
  Node * next;
};

class LRU {
  public:
    Node * head;
    Node * tail;
    
    unsigned int size;

    LRU(unsigned int maxSize);
    bool contains(unsigned long long int address);
};


class Cache {
  public:
    LRU * victimCache;
    
    LRU ** indexArray;

    unsigned int cacheSets;

    unsigned int byteOffsetBits;
    unsigned int blockOffsetBits;
    unsigned int indexBits;
    unsigned int tagBits;

    Cache(unsigned int Size, unsigned int Ways, unsigned int BlockSize);
    bool contains(unsigned long long int address);
};
