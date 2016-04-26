#include "simulator.h"
//#define PRINTSTATUS
//#define PRINTTRACE
//#define PRINTREQUEST
//#define PRINTALIGN
using namespace std;

//bool align(); //TODO
bool align(unsigned long long int requestAddress, unsigned long long int &address, unsigned int &bytesize, unsigned int busWidth, unsigned int blockSize) {
  #ifdef PRINTALIGN
  cout<<"Aligning for: "<<hex<<requestAddress<<dec<<endl;
  cout<<""<<bytesize<<" from "<<hex<<address<<dec<<endl;
  #endif
  if(bytesize > 0 && bytesize < 1024) {
    address = requestAddress + (bytesize - 1); 
    bytesize -= (address - (address & ~(busWidth-1)) + 1);
    //if(address == (address& ~(0x11))) bytesize -= 4;

    #ifdef PRINTALIGN
    cout<<""<<bytesize<<" from "<<hex<<address<<dec<<endl;
    #endif
    return true;
  }
  return false;
}

void copyNode(Node * a, Node * b) {

  b->valid = a->valid;
  b->dirty = a->dirty;
  b->address = a->address;

}

void L2Request(unsigned long long int address, Data &data, unsigned int blockSize) {
#ifdef PRINTREQUEST
  cout<<"L2 Request"<<endl;
#endif
  Node * current;
  Node temp;
  data.l2TotalRequests++;
  current = data.L2->contains(address);
  if(current) {
    //HIT L2
#ifdef PRINTSTATUS
    cout << "L2 Hit" << endl;
#endif
    data.l2HitCount++;
    data.L2->toFront(current);

  }
  else {
    current = data.L2->victimCache->contains(address);
    if(current) {
#ifdef PRINTSTATUS
      cout << "L2 VC Hit" << endl;
#endif
      //VC HIT L2
      data.l2HitCount++;
      data.l2VCHitCount++;
      data.L2->victimCache->toBack(current);
      copyNode(current, &temp);
      data.L2->push(&temp);
      temp.tag = temp.address & ~((unsigned long long int) blockSize - 1);
      data.L2->victimCache->push(&temp);
    }
    else{
#ifdef PRINTSTATUS
      cout << "L2 Miss" << endl;
#endif
      //MISS L2
      data.l2MissCount++;
      temp.valid = true;
      temp.dirty = false;
      temp.address = address;

//TODO: Transfer Alignment
      data.l2Transfers++;
      data.L2->push(&temp);
      //TODO: Handle Kickout
      if(temp.valid) {
        temp.tag = temp.address & ~((unsigned long long int) blockSize - 1);
        data.L2->victimCache->push(&temp);
      }
    }
  }
}


Data simulator(Config params) {
  
  Data data = {0};
 
  data.L1i = new Cache(params.icacheSize, params.icacheWays, params.icacheBlockSize, params.vcSize, params.addressBits);
  data.L1d = new Cache(params.dcacheSize, params.dcacheWays, params.dcacheBlockSize, params.vcSize, params.addressBits);
  data.L2  = new Cache(params.l2cacheSize, params.l2cacheWays, params.l2cacheBlockSize, params.vcSize, params.addressBits);

  char op;
  unsigned long long int address;
  unsigned int bytesize;

  unsigned long long int requestAddress;

  Node * current;
  Node temp;


  while (scanf("%c %Lx %d\n",&op,&address,&bytesize) == 3) {
    data.totalRefs++;
    switch(op) {
      case 'R':
        data.readRefs++;
        break;
      case 'W':
        data.writeRefs++;
        break;
      case 'I':
        data.instructionRefs++;
        break;
    }
    #ifdef PRINTTRACE
    cout << op << "\t" <<hex << address << dec << "\t" << bytesize << "\t" << endl;
    #endif
    requestAddress = address;
    while(align(requestAddress, address, bytesize, params.l1busWidth, 32)) {
      #ifdef PRINTREQUEST
      cout << op << "\t" <<hex << requestAddress <<"\t" << address << dec << "\t" << bytesize << "\t" << endl;
      #endif
      switch(op) {
        case 'R':
          data.l1dTotalRequests++;
          current = data.L1d->contains(address);
          if(current) {
#ifdef PRINTSTATUS
            cout << "L1d Hit" << endl;
#endif
            data.l1dHitCount++;
            data.L1d->toFront(current);
          }
          else {
            current = data.L1d->victimCache->contains(address);
            if(current) {
#ifdef PRINTSTATUS
            cout << "L1d VC Hit" << endl;
#endif
              data.l1dHitCount++;
              data.l1dVCHitCount++;
              data.L1d->victimCache->toBack(current);
              copyNode(current, &temp);
              data.L1d->push(&temp);
              temp.tag = temp.address & ~((unsigned long long int) params.dcacheBlockSize - 1);
              data.L1d->victimCache->push(&temp);
            }
            else {
#ifdef PRINTSTATUS
            cout << "L1d Miss" << endl;
#endif
              //MISS L1
              data.l1dMissCount++;
              L2Request(address, data, params.l2cacheBlockSize);

//TODO: Transfer Alignment
              data.l1dTransfers++;
              current = data.L2->contains(address);
              copyNode(current, &temp);
              temp.address = address;
              data.L1d->push(&temp);
              //TODO: Handle Kickout
              if(temp.valid) {
                temp.tag = temp.address & ~((unsigned long long int) params.dcacheBlockSize - 1);
                data.L1d->victimCache->push(&temp);
              }
            }
          }
          break;

        case 'W':
          data.l1dTotalRequests++;
          current = data.L1d->contains(address);
          if(current) {
#ifdef PRINTSTATUS
            cout << "L1d Hit" << endl;
#endif
            //HIT L1
            data.l1dHitCount++;
            data.L1d->toFront(current);
            current->dirty = true;
          }
          else {
            current = data.L1d->victimCache->contains(address);
            if(current) {
#ifdef PRINTSTATUS
              cout << "L1d VC Hit" << endl;
#endif
              //VC HIT L1
              data.l1dHitCount++;
              data.l1dVCHitCount++;
              data.L1d->victimCache->toBack(current);
              copyNode(current, &temp);
              temp.dirty = true;
              data.L1d->push(&temp);
              temp.tag = temp.address & ~((unsigned long long int) params.dcacheBlockSize - 1);
              data.L1d->victimCache->push(&temp);
            }
            else {
#ifdef PRINTSTATUS
              cout << "L1d Miss" << endl;
#endif
              //MISS L1
              data.l1dMissCount++;
              L2Request(address, data, params.l2cacheBlockSize);

//TODO: Transfer Alignment
              data.l1dTransfers++;
              current = data.L2->contains(address);
              copyNode(current, &temp);
              temp.address = address;
              temp.dirty = true;
              data.L1d->push(&temp);
              //TODO: Handle Kickout
              if(temp.valid) {
                temp.tag = temp.address & ~((unsigned long long int) params.dcacheBlockSize - 1);
                data.L1d->victimCache->push(&temp);
              }
            }
          }
          break;
        case 'I':
          data.l1iTotalRequests++;
          current = data.L1i->contains(address);
          if(current) {
#ifdef PRINTSTATUS
            cout<<"L1i Hit" << endl;
#endif
            data.l1iHitCount++;
            data.L1i->toFront(current);
            break;
          }
          else {
            current = data.L1i->victimCache->contains(address);
            if(current) {
#ifdef PRINTSTATUS
              cout<<"L1i VC Hit" << endl;
#endif
              //VC HIT L1
              data.l1iHitCount++;
              data.l1iVCHitCount++;
              data.L1i->victimCache->toBack(current);
              copyNode(current, &temp);
              data.L1i->push(&temp);
              temp.tag = temp.address & ~((unsigned long long int) params.icacheBlockSize - 1);
              data.L1i->victimCache->push(&temp);
            }
            else {
              //MISS L1
#ifdef PRINTSTATUS
              cout<<"L1i Miss" << endl;
#endif
              data.l1iMissCount++;
              L2Request(address, data, params.l2cacheBlockSize);

//TODO: Transfer Alignment
              data.l1iTransfers++;
              current = data.L2->contains(address);
              copyNode(current, &temp);
              temp.address = address;
              data.L1i->push(&temp);
              //TODO: Handle Kickout
              if(temp.valid) {
                temp.tag = temp.address & ~((unsigned long long int) params.dcacheBlockSize - 1);
                data.L1i->victimCache->push(&temp);
                if(temp.valid) {
#ifdef PRINTSTATUS
                  cout<<"L1i Kickout" << endl;
#endif
                  data.l1iKickouts++;
                }
              }
            }
          }
          break;
        
      } 
    }
  }

  return data;
}
