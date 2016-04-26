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

Data simulator(Config params) {
  
  Data data = {0};
 
  data.L1i = new Cache(params.icacheSize, params.icacheWays, params.icacheBlockSize, params.vcSize, params.addressBits);
  data.L1d = new Cache(params.dcacheSize, params.dcacheWays, params.dcacheBlockSize, params.vcSize, params.addressBits);
  data.L2  = new Cache(params.l2cacheSize, params.l2cacheWays, params.l2cacheBlockSize, params.vcSize, params.addressBits);

  char op;
  unsigned long long int address;
  unsigned int bytesize;

  unsigned long long int requestAddress;

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

          break;
        case 'W':
          data.l1dTotalRequests++;

          break;
        case 'I':
          data.l1iTotalRequests++;

          break;
      } 
    }
  }

  return data;
}
