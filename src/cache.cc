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

int LRU::access(unsigned long long int tag, unsigned long long int &address, bool dirty) {
  Node * current = head;
  Node * previous;

  unsigned int count = 0;
  unsigned long long int kickedAddress;

  while(current->valid && count < size) {
    if(current->tag == tag) {
      if(current == head) return true;
      if(tail == previous) tail = getNode(size - 2);
      if(tail == current) tail = previous;
      previous->next = current->next;
      current->next = head;
      head = current;

      return 1;
    }
    previous = current;
    previous->next = current;
    count++;
  }
  
  //TODO Handle kickouts
  if(head != current) {
    current = tail->next;
    kickedAddress = current->address;
    current->next = head;
    head = current;
    tail->next = nullptr;
    tail = getNode(size-2);
  }
  current->valid = true;
  current->dirty = dirty;//TODO
  current->tag = tag;
  current->address = address;

  return 0;
}

bool LRU::accessVC(unsigned long long int address, bool dirty) {
  return true;
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

int Cache::access(unsigned long long int address) {

  unsigned long int index = getIndex(address);
  unsigned long long int tag = getTag(address);
  unsigned long long int kickedAddress = address;

  if(indexArray[index]->access(tag, kickedAddress, false)) {
    //found in standard Cache
    return 2;
  }
  else {/*
    if(victimCache->accessVC(address, kickedAddress, false)) {
      //found in victim Cache
      return 1; 
    }*/
  }

  return 0;
}
