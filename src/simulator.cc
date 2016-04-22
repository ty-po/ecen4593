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

  while (scanf("%c %Lx %d\n",&op,&address,&bytesize) == 3) {
    data.totalRefs++;
    switch(op) {
      case 'R':
        data.readRefs++;
        data.L1d->access(address, false);
        data.L2->access(address, false);
        break;
      case 'W':
        data.writeRefs++;
        data.L1d->access(address, true);
        data.L2->access(address, true);
        break;
      case 'I':
        data.instructionRefs++;
        data.L1i->access(address, false);
        data.L2->access(address, false);
        break;
    }
  }

  return data;

}
