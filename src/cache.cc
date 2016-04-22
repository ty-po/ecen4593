#include "cache.h"

LRU::LRU(unsigned int maxSize) {
  size = 1;
  head = new Node();

  Node * next = nullptr;
  Node * current = head;
  Node * previous = nullptr;

  while(size < maxSize) {

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

  while(i < index && current->next) {
    current = current->next;
    i++;
  }

  return current;
}

bool LRU::access(unsigned long long int tag, unsigned long long int &address, bool &dirty) {
  Node * current = head;

  unsigned long long int trueAddress = address;
  bool trueDirty = dirty;

  while(current && current->valid) {
    if(current->tag == tag) {
      //found no kick; move to front
      if(current == head) return true; //Already first
      if(current == tail) tail = current->previous;
      current->previous->next = current->next;
      current->next->previous = current->previous;

      head->previous = current;
      current->next = head;
      current->previous = nullptr;

      head = current;

      return true;
    }
    current = current->next;
  }
  //not found; swap + kick
  address = 0;
  dirty = 0;

  current = tail;

  if(current->valid) {
    address = current->address;
    dirty = current->dirty;
  }

  current->tag = tag;
  current->address = trueAddress;
  current->valid = true;
  current->dirty = trueDirty; 
  
  if(current->previous) {
    tail = current->previous;
    current->previous->next = nullptr;

  }


  if(head->next) {
    head->previous = current;
    current->next = head->next;
    current->previous = nullptr;
    head = current;
  }

  return false;
}

bool LRU::accessVC(unsigned long long int address, unsigned long long int &kickedAddress, bool &dirty, unsigned int blockOffsetBits) {
  Node * current = head;

  unsigned long long int tag = (address>>blockOffsetBits)<<blockOffsetBits;
  unsigned long long int kickedTag = (kickedAddress>>blockOffsetBits)<<blockOffsetBits;

  bool trueDirty = dirty;
  unsigned long long int trueKicked = kickedAddress;

  while(current && current->valid) {
    if(current->tag == tag){
      //found in VC; remove, push kicked and pass back dirty
      if(current == head) return true; //Already first
      current->previous->next = current->next;
      current->next->previous = current->previous;

      head->previous = current;
      current->next = head;
      current->previous = nullptr;

      dirty = current->dirty;
      current->dirty = trueDirty;

      current->address = kickedAddress;
      current->tag = kickedTag;

      head = current;
      tail = tail->previous;

      return true;
    }
    current = current->next;
  }
  //not in VC; add Kicked and compute kickout; pass back dirty and address
  
  kickedAddress = 0;
  dirty = 0;

  current = head;

  if(tail) {
    current = tail;
    current->previous->next = nullptr;
    tail = current->previous;

    head->previous = current;
    current->next = head;
    head = current;
    current->previous = nullptr;
  }

  kickedAddress = current->address;
  dirty = current->dirty;

  current->tag = kickedTag;
  current->address = trueKicked;
  current->valid = true;
  current->dirty = trueDirty;

  return false;
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

int Cache::access(unsigned long long int address, bool write) {

  unsigned long int index = getIndex(address);
  unsigned long long int tag = getTag(address);
  unsigned long long int kickedAddress = address;

  bool dirty = write;
  //STILL GOING TO BE BROKEN
  if(indexArray[index]->access(tag, kickedAddress, dirty)) {
    //found in standard Cache
    //HIT - done
    return 0;
  }

  //not found in standard
  if(kickedAddress == 0) {
    //NOT in VC
    //MISS NO KICKOUT - done
    return 1;
  }

  //may be in VC
  if(victimCache->accessVC(address, kickedAddress, dirty, blockOffsetBits)) {
    //found in VC
    //remove found from LRU, add kicked
    //mark block as dirty/not in normal :TODO:
  //indexArray[kickedIndex]->markDirty(tag, kickedAddress);
    //HIT NO KICKOUT
    return 0;
  }  

  //not in VC
  //add kickedAddress to LRU
  //KICKOUT
  if(dirty) return 3; //dirty kickout return Address somehow?
  return 2; //clean kickout
}     
