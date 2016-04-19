#include "cache.h"

LRU::LRU(unsigned int maxSize) {
  size = 1;
  head = new Node();

  Node * current = head;
  Node * previous;

  while(size < maxSize) {
    previous = current;
    current = new Node();
    previous->next = current;;
    size++;
  }
  tail = previous;
}

Cache::Cache(unsigned int Size, unsigned int Ways, unsigned int BlockSize) {
  victimCache = new LRU(8);

  cacheSets = Size/BlockSize;

  indexArray = new LRU * [cacheSets];

  int i;
  for(i = 0; i < cacheSets; i++) {
    indexArray[i] = new LRU(Ways);
  }

}
