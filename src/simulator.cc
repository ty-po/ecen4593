#include "simulator.h"

Data simulator(Config params) {
  
  Data data = {0};
  
  char op;
  unsigned long long int address;
  unsigned int bytesize;

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
  }

  return data;

}
