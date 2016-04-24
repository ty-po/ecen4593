#include "simulator.h"

void align();


Data simulator(Config params) {
  
  Data data = {0};
 
  data.L1i = new Cache(params.icacheSize, params.icacheWays, params.icacheBlockSize, params.vcSize, params.addressBits);
  data.L1d = new Cache(params.dcacheSize, params.dcacheWays, params.dcacheBlockSize, params.vcSize, params.addressBits);
  data.L2  = new Cache(params.l2cacheSize, params.l2cacheWays, params.l2cacheBlockSize, params.vcSize, params.addressBits);

  char op;
  unsigned long long int address;
  unsigned int bytesize;

  int rv;
  int l2;

  while (scanf("%c %Lx %d\n",&op,&address,&bytesize) == 3) {
    data.totalRefs++;
    std::cout << op << "\t" << std::hex << address << std::dec << "\t" << bytesize << std::endl;
    switch(op) {
      case 'R':
        data.readRefs++;
        data.l1dTotalRequests++;
        rv = data.L1d->access(address, false);
        break;
      case 'W':
        data.writeRefs++;
        data.l1dTotalRequests++;
        rv = data.L1d->access(address, true);
        break;
      case 'I':
        data.instructionRefs++;
        data.l1iTotalRequests++;
        rv = data.L1i->access(address, false);
        break;
    }

    switch(rv) { 
      case 0:
        std::cout<<"Hit Main"<<std::endl;
        break;
      case 1:
        std::cout<<"Hit VC"<<std::endl;
        break;
      case 2:
        std::cout<<"Kickout";
        //data.L2->access(,true);
      case 3:
        data.l2TotalRequests++;
        std::cout<<"Miss Main Unfilled ";
        l2 = data.L2->access(address, false);
        std::cout<<l2<<std::endl;
        break;
    }
  }

  return data;

}
