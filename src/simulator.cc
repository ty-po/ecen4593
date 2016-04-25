#include "simulator.h"
//#define PRINTSTATUS
//#define PRINTTRACE
using namespace std;

void align(); //TODO


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
    #ifdef PRINTTRACE
    cout << op << "\t" <<hex << address << dec << "\t" << bytesize << "\t";
    #endif
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
        //data.L2->access(kickedAddress,true);//TODO
        break;
      case 3:
        data.l2TotalRequests++;
        #ifdef PRINTSTATUS
        cout<<"Miss L1 - Main Unfilled "<<endl;
        #endif

        l2 = data.L2->access(address, false);
        #ifdef PRINTSTATUS
        cout<<"\tL2 request status"<<l2<<endl;
        #endif
        break;
    }
  }

  return data;

}
