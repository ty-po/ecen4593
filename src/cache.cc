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

void LRU::markDirty() {
  head->dirty = true;
}

bool LRU::access(unsigned long long int tag, unsigned long long int &address, bool &dirty) {
  #ifdef DEBUG
  std::cout<<std::hex<<"\t"<<tail<<std::dec<<std::endl;
  std::cout<<std::hex<<"\t"<<current<<std::dec<<std::endl;
  std::cout<<std::hex<<"\t"<<head<<std::dec<<std::endl;
  std::cout<<"Access for Tag: "<<std::hex<<tag<<std::dec<<std::endl;
  #endif
  Node * current = head;
  unsigned long long int trueAddress = address;
  bool trueDirty = dirty;
  while(current && current->valid) {
  #ifdef DEBUG
  std::cout<<"SC loop: "<<std::hex<<current->tag<<std::endl;
  #endif
    if(current->tag == tag) {
      //found no kick; move to front

      toFront(current);

      current->dirty = trueDirty;
      return true;
    }
    current = current->next;
  }
  //not found; swap + kick
  #ifdef DEBUG
  std::cout<<"Not in main cache"<<std::endl;
  #endif
  address = 0;
  dirty = 0;

  current = tail;

  if(current->valid) {
    address = current->address;
    dirty = current->dirty;
  }

  toFront(current);

  current->tag = tag;
  current->address = trueAddress;
  current->valid = true;
  current->dirty = trueDirty; 

  return false;
}

bool LRU::accessVC(unsigned long long int address, unsigned long long int &kickedAddress, bool &dirty, unsigned int blockOffsetBits) {
  #ifdef DEBUG
  std::cout<<std::hex<<tail<<std::dec<<std::endl;
  std::cout<<std::hex<<current<<std::dec<<std::endl;
  std::cout<<std::hex<<head<<std::dec<<std::endl;
  std::cout<<"VC access Address: "<<std::hex<<address<<std::endl;
  #endif
  Node * current = head;

  unsigned long long int tag = (address>>blockOffsetBits)<<blockOffsetBits;
  unsigned long long int kickedTag = (kickedAddress>>blockOffsetBits)<<blockOffsetBits;

  bool trueDirty = dirty;
  unsigned long long int trueKicked = kickedAddress;

  while(current && current->valid) {
    #ifdef DEBUG
    std::cout<<"VC loop:"<<std::hex<<current->tag<<std::dec<<std::endl;
    #endif
    if(current->tag == tag){
      //found in VC; remove, push kicked and pass back dirty
      #ifdef DEBUG
      std::cout<<"found in VC"<<std::endl;
      #endif
      
      toFront(current);
      
      dirty = current->dirty;

      current->dirty = trueDirty;

      current->address = trueKicked;
      current->tag = kickedTag;

      return true;
    }
    current = current->next;
  }
  //not in VC; add Kicked and compute kickout; pass back dirty and address
  #ifdef DEBUG
  std::cout<<"not in VC"<<std::endl;
  #endif
  address = 0;
  dirty = 0;

  current = tail;

  if(current->valid) {
    address = current->address;
    dirty = current->dirty;
  }
  
  toFront(current);

  current->tag = tag;
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

int Cache::access(unsigned long long int address, bool write, unsigned long long int writeback) {

  unsigned long int index = getIndex(address);
  unsigned long long int tag = getTag(address);
  unsigned long long int kickedAddress = address;

  bool dirty = write;

  if(indexArray[index]->access(tag, kickedAddress, dirty)) {
    //found in standard Cache
    //HIT - done
    return 0;
  }

  //not found in standard
  if(kickedAddress == 0) {
    //NOT in VC
    //MISS NO KICKOUT - done
    return 3;
  }

  //may be in VC
  dirty = write;
  if(victimCache->accessVC(address, kickedAddress, dirty, blockOffsetBits)) {
    //found in VC
    //remove found from LRU, add kicked
    if(dirty) indexArray[index]->markDirty();
    //HIT NO KICKOUT
    return 1;
  }

  //not in VC
  //add kickedAddress to LRUi

  //KICKOUT
  writeback = kickedAddress;
  return 2; //dirty kickout TODO return dirty and address
}     
