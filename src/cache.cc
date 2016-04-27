#include "cache.h"
//#define DEBUG
using namespace std;

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

void LRU::toBack(Node * current) {
  if(current == tail) return;
  if(current == head) {
    head = current->next;
    head->previous = nullptr;
    current->next = nullptr;

    current->previous = tail;
    
    tail->next = current;

    tail = current;

    return;
  }
  current->next->previous = current->previous;
  current->previous->next = current->next;

  current->previous = tail;
  current->next = nullptr;

  tail->next = current;

  tail = current;
}

void copy(Node * a, Node * b) {
  b->index = a->index;
  b->tag = a->tag;
  b->address = a->address;
  b->valid = a->valid;
  b->dirty = a->dirty;
}


Node * LRU::contains(unsigned long long int tag) {
  Node * current = head;
  while(current && current->valid) {
#ifdef DEBUG
    cout << "Loop " << hex << current->tag << "\tt:" << tag << endl;
#endif
    if(current->tag == tag) {
#ifdef DEBUG
      cout << " FOUND " << !!(current) <<endl;
#endif
      return current;
    }
    current = current->next;
  }
#ifdef DEBUG
  cout<<" NOT FOUND " << !!(nullptr)<<endl;
#endif
  return nullptr;
}

bool LRU::push(Node * current) {
  Node tmp;

  copy(current, &tmp);
  copy(tail, current);
  copy(&tmp, tail);
  
  toFront(tail);

  return current->valid;
}

void LRU::remove(Node * current) {
  toBack(current);
  current->valid = false;
  current->dirty = false;
  current->address = 0;
  current->tag = 0;
  current->index = 0;
}

void LRU::printLRU() {
  int i = 0;
  Node * current = getNode(i);
  cout << "--------------------------------" << endl;
  while(current){
    cout << hex << "i:" << current ->index << "\tt:" << current->tag << endl;
    cout << "a:" << current->address << "\tv:" << current->valid << "\td:" << current->dirty << endl;
    cout << "^^ " << current->previous << " | " << current->next << " \\/" << endl << endl;
    i++;
    current = getNode(i);
  }
  cout << "--------------------------------" << endl;
  cout << "h:" << head << "\tt:" << tail << endl;
  cout << "--------------------------------" << endl << endl;
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

  #ifdef DEBUG
  cout <<cacheSets <<endl;
  cout << blockOffsetBits <<endl;
  cout << indexBits <<endl;
  cout << tagBits <<endl;
  #endif

  indexArray = new LRU * [cacheSets];

  int i;
  for(i = 0; i < cacheSets; i++) {
    indexArray[i] = new LRU(Ways);
  }

}

unsigned int Cache::getIndex(unsigned long long int address) {
  return (address<<tagBits)>>(blockOffsetBits + tagBits);
}

unsigned long long int Cache::getTag(unsigned long long int address) {
  return address>>(blockOffsetBits + indexBits);
}

Node * Cache::contains(unsigned long long int address) {
#ifdef LRUDEBUG
  cout << hex << "i:" <<getIndex(address) << "\tt:" << getTag(address) <<dec<<endl;
#endif
  return indexArray[getIndex(address)]->contains(getTag(address));
}

void Cache::toFront(Node * current) {
  indexArray[getIndex(current->address)]->toFront(current);
#ifdef LRUDEBUG
  indexArray[getIndex(current->address)]->printLRU();
#endif
}

Node * Cache::head(unsigned long long int address) {
  return indexArray[getIndex(address)]->head;
}


Node * Cache::tail(unsigned long long int address) {
  return indexArray[getIndex(address)]->tail;
}

bool Cache::push(Node * current) {
  current->tag = getTag(current->address);
  current->index = getIndex(current->address);
  return indexArray[current->index]->push(current);
}

void Cache::printCache(string cacheName, unsigned int vcSize) {
  unsigned int i;
  unsigned int j; 


  LRU * currentLRU;
  Node * currentNode;

  cout << "Memory Lvl: " << cacheName << endl;
  for(i = 0; i < cacheSets; i++) {
    currentLRU = indexArray[i];
    if(currentLRU->head->valid) {
      cout << "Index: " << setw(4)<<hex << i << dec;
      for(j = 0; j < ways; j++) {
        currentNode = currentLRU->getNode(j);
        if(!(j%2) && j) cout << setw(12)<<"";
        if(!j) cout << " ";

        if(currentNode && currentNode->valid) {
          cout << " | V:" << currentNode->valid;
          cout << " D:" << currentNode->dirty;
          cout << " Tag: ";
          cout << setw(12)<<hex << currentNode->tag << dec;
        }
        else cout << " | V:0 D:0 Tag: " << setw(12) << "-";


        if(j%2 || j == (ways - 1)) {
          cout << " |" << endl;
        }

      }
    }
  }


  cout << "Victim cache:" << endl;
  currentLRU = victimCache;
  for(i = 0; i < vcSize; i++) {
    currentNode = currentLRU->getNode(i);
    if(!(i%2)) {
      cout << setw(11)<<"";
    }
    if(currentNode && currentNode->valid) {
      cout << " | V:" << currentNode->valid;
      cout << " D:" << currentNode->dirty;
      cout << " Addr: ";
      cout << setw(12)<<hex << currentNode->tag << dec;
    }
    else cout << " | V:0 D:0 Addr: " << setw(12) << "-";

    if(i%2) {
      cout << " |" << endl;
    }
  }

}
