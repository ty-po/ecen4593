#include "cache.h"
//#define DEBUG

LRU::LRU(unsigned int maxSize) {
  size = 1;
  head = new Node();

  Node * next = nullptr;
  Node * current = head;
  Node * previous = nullptr;

  while(size <= maxSize) {

    current->previous = previous;
    next = new Node();
    current->next = next;

    previous = current;
    current = next;
    
    size++;
  }

  current->previous = previous;
  current->next = nullptr;

  tail = current;

  //tail->next->next = nullptr;
}

Node * LRU::getNode(unsigned int index) {
  Node * current = head;
  unsigned int i = 0;
  while(current) {
    if(i == index) return current;
    current = current->next;
    i++;
  }
  return nullptr;
}

void LRU::toFront(Node * current) {
  if(current == head) return;
  if(current == tail) {
    tail = current->previous;
    tail->next = nullptr;
    current->previous = nullptr;

    current->next = head;
    
    head->previous = current;

    head = current;

    return;
  }
  current->next->previous = current->previous;
  current->previous->next = current->next;

  current->previous = nullptr;
  current->next = head;

  head->previous = current;

  head = current;
}

Node * LRU::contains(unsigned long long int tag) {

}

unsigned int log2(unsigned int x) {
  unsigned int rv = 0;
  while (x >>= 1) rv++;
  return rv;
}

Cache::Cache(unsigned int Size, unsigned int Ways, unsigned int BlockSize, unsigned int vcSize, unsigned int addressBits) {
  victimCache = new LRU(vcSize);

  cacheSets = (Size/BlockSize)/Ways;
  ways = Ways;

  blockOffsetBits = log2(BlockSize);
  indexBits = log2(cacheSets);
  tagBits = addressBits - (blockOffsetBits + indexBits); 

  indexArray = new LRU * [cacheSets];

  int i;
  for(i = 0; i < cacheSets; i++) {
    indexArray[i] = new LRU(Ways);
  }

}

unsigned long int Cache::getIndex(unsigned long long int address) {
  return (address<<tagBits)>>(blockOffsetBits + tagBits);
}

unsigned long long int Cache::getTag(unsigned long long int address) {
  return address>>(blockOffsetBits + indexBits);
}

Node * Cache::contains(unsigned long long int address) {

}
