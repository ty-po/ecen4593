#include "simulator.h"
#define PRINTSTATUS
//#define PRINTTRACE
#define PRINTALIGN
using namespace std;

//bool align(); //TODO
bool align(unsigned long long int requestAddress, unsigned long long int &address, unsigned int &bytesize, unsigned int busWidth, unsigned int blockSize) {
  /*
  #ifdef PRINTALIGN
  cout<<"Aligning for: "<<hex<<requestAddress<<dec<<endl;
  cout<<""<<bytesize<<" from "<<hex<<address<<dec<<endl;
  #endif
  if(bytesize > 0 && bytesize < 500000) {
    address = requestAddress + (bytesize - 1); 
    bytesize -= (address - (address & ~(0x11)));

    #ifdef PRINTALIGN
    cout<<""<<bytesize<<" from "<<hex<<address<<dec<<endl;
    #endif
    return true;
  }
  return false;
  */
  bool rv = false;
  if (bytesize) rv = true;
  bytesize = 0;
  return rv;
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

  int rv;
  int l2;

  unsigned long long int kickedAddress;

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
          rv = data.L1d->access(address, false, kickedAddress);
          break;
        case 'W':
          data.l1dTotalRequests++;
          rv = data.L1d->access(address, true, kickedAddress);
          break;
        case 'I':
          data.l1iTotalRequests++;
          rv = data.L1i->access(address, false, kickedAddress);
          break;
      } 
      #ifdef PRINTSTATUS
      cout << "\t";
      #endif
      switch(rv) { 
        case 0:
          #ifdef PRINTSTATUS
          cout<<"Hit L1 Main"<<endl;
          #endif
          break;
        case 1:
          #ifdef PRINTSTATUS
          cout<<"Hit L1 VC"<<endl;
          #endif
          break;
        case 2:
          #ifdef PRINTSTATUS
          cout<<"L1 Kickout"<<endl;
          #endif
          data.L2->access(kickedAddress,true, kickedAddress);//TODO
          break;
        case 3:
          data.l2TotalRequests++;
          #ifdef PRINTSTATUS
          cout<<"Miss L1 - Main Unfilled "<<endl;
          #endif

          l2 = data.L2->access(address, false, kickedAddress);
          #ifdef PRINTSTATUS
          cout<<"\tL2 request status "<<l2<<endl;
          #endif
          break;
      }
    }
  }

  return data;
}
