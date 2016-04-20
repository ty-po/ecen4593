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

Node * LRU::getNode(unsigned int index) {
  Node * current = head;
  unsigned int i = 0;

  while(i < index && current->next) {
    current = current->next;
  }

  return current;
}



Cache::Cache(unsigned int Size, unsigned int Ways, unsigned int BlockSize, unsigned int vcSize) {
  victimCache = new LRU(vcSize);

  cacheSets = Size/BlockSize;
  ways = Ways;

  indexArray = new LRU * [cacheSets];

  int i;
  for(i = 0; i < cacheSets; i++) {
    indexArray[i] = new LRU(Ways);
  }

}
