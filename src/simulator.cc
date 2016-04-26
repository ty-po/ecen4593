#include "simulator.h"
//#define PRINTSTATUS
//#define PRINTTRACE
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
    cout << op << "\t" <<hex << address << dec << "\t" << bytesize << "\t";
    #endif
    requestAddress = address;
    while(align(requestAddress, address, bytesize, params.l1busWidth, 32)) {
      switch(op) {
        case 'R':
          data.l1dTotalRequests++;
          current = data.L1d->contains(address);
          if(current) {
            //HIT L1
            data.l1dHitCount++;
            data.L1d->toFront(current);
          }
          else {
            current = data.L1d->victimCache->contains(address);
            if(current) {
              //VC HIT L1
              data.l1dHitCount++;
              data.l1dVCHitCount++;
              data.L1d->victimCache->toBack(current);
              data.L1d->push(current);
              copyNode(current, &temp);
              temp.tag = temp.address & ~(0x1F);
              data.L1d->victimCache->push(&temp);
            }
            else {
              //MISS L1
              data.l1dMissCount++;
              current = data.L2->contains(address);
              if(current) {
                //HIT L2
                data.l2HitCount++;
                data.L2->toFront(current);

              }
              else {
                current = data.L2->victimCache->contains(address);
                if(current) {
                  //VC HIT L2
                  data.l2HitCount++;
                  data.l2VCHitCount++;
                  data.L2->victimCache->toBack(current);
                  data.L2->push(current);
                  copyNode(current, &temp);
                  temp.tag = temp.address & ~(0x1F);
                  data.L2->victimCache->push(&temp);
                }
                else{
                  //MISS L2
                  data.l2MissCount++;
                  temp.valid = true;
                  temp.dirty = false;
                  temp.address = address;
      
//TODO: Transfer Alignment
                  //
                  data.L2->push(&temp);
                  //TODO: Handle Kickout
                }
              }

//TODO: Transfer Alignment
              //
              current = data.L2->contains(address);
              data.L1d->push(current);
              //TODO: Handle Kickout
            }
          }
          break;

        case 'W':
          data.l1dTotalRequests++;
          current = data.L1i->contains(address);
          if(current) {

          }
          else {

          }
          break;
        case 'I':
          data.l1iTotalRequests++;
          current = data.L1i->contains(address);
          if(current) {

          }
          else {

          }
          break;
        
      } 
    }
  }

  return data;
}
