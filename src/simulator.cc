#include "simulator.h"
//#define PRINTSTATUS
//#define PRINTTRACE
//#define PRINTREQUEST
//#define PRINTALIGN
using namespace std;

//bool align(); //TODO
bool align(unsigned long long int requestAddress, unsigned int requestBytesize, unsigned long long int &address, unsigned int &bytesize, unsigned int busWidth, unsigned int blockSize) {
  unsigned long long int mask = ~((unsigned long long int)busWidth-1);


  if(bytesize > 1024 || bytesize == 0) return false;
  if(bytesize != requestBytesize) {
    address = ((address & mask) + 4);
  }
  bytesize -= ((address & mask) + 4) - address;
  return true;

}

void copyNode(Node * a, Node * b) {

  b->valid = a->valid;
  b->dirty = a->dirty;
  b->address = a->address;

}

void L2Request(unsigned long long int address, Data &data, Config params, unsigned long long int &cycles, bool writeback = false) {

  unsigned int blockSize = params.l2cacheBlockSize;
#ifdef PRINTREQUEST
  cout<<"L2 Request"<<endl;
#endif
  Node * current;
  Node temp;
  unsigned long long int l2VCMask = ~((unsigned long long int) blockSize - 1);

  data.l2TotalRequests++;
  current = data.L2->contains(address);
  if(current) {
    //HIT L2
#ifdef PRINTSTATUS
    cout << "L2 Hit" << endl;
#endif
    data.l2HitCount++;
    cycles += params.l2hitTime;
    data.L2->toFront(current);
    //if(writeback) current->dirty = true;
    //TODO
    current->dirty = writeback;
  }
  else {
    current = data.L2->victimCache->contains(address & l2VCMask);
    if(current) {
#ifdef PRINTSTATUS
      cout << "L2 VC Hit" << endl;
#endif
      //VC HIT L2
      data.l2MissCount++;
      data.l2VCHitCount++;
      cycles += params.l2missTime;
      data.L2->victimCache->toBack(current);
      copyNode(current, &temp);
      temp.address = address;
      //if(writeback) temp.dirty = true;
      //TODO
      temp.dirty = writeback;
      data.L2->push(&temp);
      cycles += params.l2hitTime;
      temp.tag = temp.address & l2VCMask;
      data.L2->victimCache->push(&temp);
    }
    else{
#ifdef PRINTSTATUS
      cout << "L2 Miss" << endl;
#endif
      //MISS L2
      data.l2MissCount++;
      cycles += params.l2missTime;


      if(data.L2->tail(address)->valid && data.L2->victimCache->tail->valid) {
#ifdef PRINTSTATUS
        cout<<"L2 Kickout" << endl;
#endif
        data.l2Kickouts++;

        if(data.L2->victimCache->tail->dirty) {
#ifdef PRINTSTATUS
          cout<<"L2 Dirty Kickout" << endl;
#endif
          data.l2DirtyKickouts++;
          //TODO Dirty Kickout
          cycles += params.memorySendAddressTime;
          cycles += params.memoryReadyTime;
          cycles += params.chunkTime * (params.l2cacheBlockSize/params.chunkSize);

        }
      }

      //Memory Request

//TODO: Transfer Alignment
      data.l2Transfers++;
      cycles += params.memorySendAddressTime;
      cycles += params.memoryReadyTime;
      cycles += params.chunkTime * (params.l2cacheBlockSize/params.chunkSize);

      temp.valid = true;
      temp.dirty = writeback;
      temp.address = address;

      data.L2->push(&temp);
      cycles += params.l2hitTime;

      if(temp.valid) {
        temp.tag = temp.address & l2VCMask;
        data.L2->victimCache->push(&temp);
      }



/*
      temp.valid = true;
      temp.dirty = writeback;
      temp.address = address;

//TODO: Transfer Alignment
      data.l2Transfers++;
      cycles += params.memorySendAddressTime;
      cycles += params.memoryReadyTime;
      cycles += params.chunkTime * (params.l2cacheBlockSize/params.chunkSize);
      data.L2->push(&temp);
      cycles += params.l2hitTime;
      //TODO: Handle Kickout
      if(temp.valid) {
        temp.tag = temp.address & l2VCMask;
        data.L2->victimCache->push(&temp);
        if(temp.valid) {
#ifdef PRINTSTATUS
          cout << "L2 Kickout" << endl;
#endif  
          data.l2Kickouts++;
          if(temp.dirty) {
#ifdef PRINTSTATUS
          cout << "L2 Dirty Kickout" << endl;
#endif  
            data.l2DirtyKickouts++;
            cycles += params.memorySendAddressTime;
            cycles += params.memoryReadyTime;
            cycles += params.chunkTime * (params.l2cacheBlockSize/params.chunkSize);
          }
        }
      }*/
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
  unsigned int requestBytesize;

  unsigned long long int dcacheVCMask = ~((unsigned long long int) params.dcacheBlockSize - 1);
  unsigned long long int icacheVCMask = ~((unsigned long long int) params.icacheBlockSize - 1);

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
        data.instructionCycles++;
        break;
    }
    #ifdef PRINTTRACE
    cout << op << "\t" <<hex << address << dec << "\t" << bytesize << "\t" << endl;
    #endif
    requestAddress = address;
    requestBytesize = bytesize;
    while(align(requestAddress, requestBytesize, address, bytesize, params.l1busWidth, 32)) {
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
            data.readCycles += params.l1hitTime;
            data.L1d->toFront(current);
          }
          else {
            current = data.L1d->victimCache->contains(address & dcacheVCMask);
            if(current) {
#ifdef PRINTSTATUS
            cout << "L1d VC Hit" << endl;
#endif
              data.l1dMissCount++;
              data.l1dVCHitCount++;
              data.readCycles += params.l1missTime;
              data.L1d->victimCache->toBack(current);
              copyNode(current, &temp);
              temp.address = address;
              data.L1d->push(&temp);
              data.readCycles += params.l1hitTime;
              temp.tag = temp.address & dcacheVCMask;
              data.L1d->victimCache->push(&temp);
            }
            else {
#ifdef PRINTSTATUS
            cout << "L1d Miss" << endl;
#endif
              //MISS L1
              data.l1dMissCount++;
              data.readCycles += params.l1missTime;
              
              if(data.L1d->tail(address)->valid && data.L1d->victimCache->tail->valid) {
#ifdef PRINTSTATUS
                cout<<"L1d Kickout" << endl;
#endif
                data.l1dKickouts++;

                if(data.L1d->victimCache->tail->dirty) {
#ifdef PRINTSTATUS
                  cout<<"L1d Dirty Kickout" << endl;
#endif
                  data.l1dDirtyKickouts++;
                  L2Request(data.L1d->victimCache->tail->address, data, params, data.writeCycles, true);

                }
              }

              //TODO: Handle Kickout
              L2Request(address, data, params, data.readCycles);

//TODO: Transfer Alignment
              data.l1dTransfers++;
              data.readCycles += params.l2transferTime * (params.dcacheBlockSize/params.l2busWidth);
              current = data.L2->head(address);
              copyNode(current, &temp);
              temp.address = address;
              data.L1d->push(&temp);
              data.readCycles += params.l1hitTime;
              current->dirty = false;
              if(temp.valid) {
                temp.tag = temp.address & dcacheVCMask;
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
            data.writeCycles += params.l1hitTime;
            data.L1d->toFront(current);
            current->dirty = true;
          }
          else {
            current = data.L1d->victimCache->contains(address & dcacheVCMask);
            if(current) {
#ifdef PRINTSTATUS
              cout << "L1d VC Hit" << endl;
#endif
              //VC HIT L1
              data.l1dMissCount++;
              data.l1dVCHitCount++;
              data.writeCycles += params.l1missTime;
              data.L1d->victimCache->toBack(current);
              copyNode(current, &temp);
              temp.address = address;
              temp.dirty = true;
              data.L1d->push(&temp);
              data.writeCycles += params.l1hitTime;
              temp.tag = temp.address & dcacheVCMask;
              data.L1d->victimCache->push(&temp);
            }
            else {
#ifdef PRINTSTATUS
              cout << "L1d Miss" << endl;
#endif
              //MISS L1
              data.l1dMissCount++;
              data.writeCycles += params.l1missTime;

              if(data.L1d->tail(address)->valid && data.L1d->victimCache->tail->valid) {
#ifdef PRINTSTATUS
                cout<<"L1d Kickout" << endl;
#endif
                data.l1dKickouts++;

                if(data.L1d->victimCache->tail->dirty) {
#ifdef PRINTSTATUS
                  cout<<"L1d Dirty Kickout" << endl;
#endif
                  data.l1dDirtyKickouts++;
                  L2Request(data.L1d->victimCache->tail->address, data, params, data.writeCycles, true);

                }
              }

              //TODO: Handle Kickout
              L2Request(address, data, params, data.writeCycles);

//TODO: Transfer Alignment
              data.l1dTransfers++;
              data.writeCycles += params.l2transferTime * (params.dcacheBlockSize/params.l2busWidth);
              current = data.L2->head(address);
              copyNode(current, &temp);
              temp.address = address;
              temp.dirty = true;
              data.L1d->push(&temp);
              data.writeCycles += params.l1hitTime;
              current->dirty = false;
              if(temp.valid) {
                temp.tag = temp.address & dcacheVCMask;
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
            data.instructionCycles += params.l1hitTime;
            data.l1iHitCount++;
            data.L1i->toFront(current);
            break;
          }
          else {
            current = data.L1i->victimCache->contains(address & icacheVCMask);
            if(current) {
#ifdef PRINTSTATUS
              cout<<"L1i VC Hit" << endl;
#endif
              //VC HIT L1
              data.l1iMissCount++;
              data.l1iVCHitCount++;
              data.instructionCycles += params.l1missTime;
              data.L1i->victimCache->toBack(current);
              copyNode(current, &temp);
              temp.address = address;
              data.L1i->push(&temp);
              data.instructionCycles += params.l1hitTime;
              temp.tag = temp.address & icacheVCMask;
              data.L1i->victimCache->push(&temp);
            }
            else {
              //MISS L1
#ifdef PRINTSTATUS
              cout<<"L1i Miss" << endl;
#endif
              data.l1iMissCount++;
              data.instructionCycles += params.l1missTime;

              if(data.L1i->tail(address)->valid && data.L1i->victimCache->tail->valid) {
#ifdef PRINTSTATUS
                cout<<"L1i Kickout" << endl;
#endif
                data.l1iKickouts++;

                if(data.L1i->victimCache->tail->dirty) {
#ifdef PRINTSTATUS
                  cout<<"L1i Dirty Kickout" << endl;
#endif
                  data.l1iDirtyKickouts++;
                  L2Request(data.L1i->victimCache->tail->address, data, params, data.writeCycles, true);

                }
              }

              //TODO: Handle Kickout
              L2Request(address, data, params, data.instructionCycles);

//TODO: Transfer Alignment
              data.l1iTransfers++;
              data.instructionCycles += params.l2transferTime * (params.dcacheBlockSize/params.l2busWidth);
              current = data.L2->head(address);
              copyNode(current, &temp);
              temp.address = address;
              data.L1i->push(&temp);
              data.instructionCycles += params.l1hitTime;

              if(temp.valid) {
                temp.tag = temp.address & icacheVCMask;
                data.L1i->victimCache->push(&temp);
              }




/*
              L2Request(address, data, params, data.instructionCycles);

//TODO: Transfer Alignment
              data.l1iTransfers++;
              data.instructionCycles += params.l2transferTime * (params.icacheBlockSize/params.l2busWidth);
              current = data.L2->contains(address);
              copyNode(current, &temp);
              temp.address = address;
              data.L1i->push(&temp);
              data.instructionCycles += params.l1hitTime;
              if(current->dirty) current->dirty = false;
              //TODO: Handle Kickout
              if(temp.valid) {
                temp.tag = temp.address & icacheVCMask;
                data.L1i->victimCache->push(&temp);
                if(temp.valid) {
#ifdef PRINTSTATUS
                  cout<<"L1i Kickout" << endl;
#endif
                  data.l1iKickouts++;
                }
              }*/
            }
          }
          break;
        
      } 
    }
  }
  data.totalCycles = data.readCycles + data.writeCycles + data.instructionCycles;
  data.idealTime = (2 * data.instructionRefs) + data.readRefs + data.writeRefs;
  data.idealMisAlignedTime = data.instructionRefs + data.l1iTotalRequests + data.l1dTotalRequests;
  return data;
}
