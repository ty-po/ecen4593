#include "cache.h"

LRU::LRU(unsigned int maxSize) {
  size = 1;
  head = new Node();

  Node * current = head;
  Node * previous;

  while(size < maxSize) {
    previous = current;
    current = new Node();
    previous->next = current;
    size++;
  }
  tail = previous;
  //tail->next->next = nullptr;
}

Node * LRU::getNode(unsigned int index) {
  Node * current = head;
  unsigned int i = 0;

  while(i < index && current->next) {
    current = current->next;
  }

  return current;
}

bool LRU::access(unsigned long long int address) {
  Node * current = head;
  Node * previous;

  unsigned int count = 0;

  while(current->valid && count < size) {
    if(current->address == address) {
      if(current == head) return true;
      if(tail == previous) tail = getNode(size - 2);
      if(tail == current) tail = previous;
      previous->next = current->next;
      current->next = head;
      head = current;

      return 0;
    }
    previous = current;
    previous->next = current;
    count++;
  }
  
  //TODO Handle kickouts
  if(head != current) {
    current = tail->next;
    current->next = head;
    head = current;
    tail->next = nullptr;
    tail = getNode(size-2);
  }
  current->valid = true;
  current->dirty = false;//TODO
  current->address = address;

  return 1;
}

unsigned int log2(unsigned int x) {
  unsigned int rv = 0;
  while (x >>= 1) rv++;
  return rv;
}

Cache::Cache(unsigned int Size, unsigned int Ways, unsigned int BlockSize, unsigned int vcSize, unsigned int addressBits) {
  victimCache = new LRU(vcSize);

  cacheSets = Size/BlockSize;
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

unsigned int Cache::access(unsigned long long int address) {
  unsigned long int index = getIndex(address);
  unsigned long long int tag = getTag(address);

  if(indexArray[index]->access(tag)) return 0;
  if(victimCache->access(address)) return 1;//Def not right
  return 2;
}
