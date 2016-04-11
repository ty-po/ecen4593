#include "output.h"
using namespace std;

void help() {

}

void output(Config params, Data data) {
  cout << endl;
  cout << "-------------------------------------------------------------------------" << endl;
  cout << setw(6)<<"" << left << setw(3)<< "tr1"/*params.trace*/ << "." << setw(16)<<"default"/*params.setup*/ << "Simulation Results" << endl;
  cout << "-------------------------------------------------------------------------" << endl;
  cout << endl;

  cout << setw(2)<<"" << "Memory system:" << endl;
  cout << setw(4)<<"" << "Dcache size = " << 8192/*params.dcacheSize*/ << " : ways = " << 1/*params.dcacheWays*/ << " : block size = " << 32/*params.dcacheBlockSize*/ << endl;
  cout << setw(4)<<"" << "Icache size = " << 8192/*params.icacheSize*/ << " : ways = " << 1/*params.icacheWays*/ << " : block size = " << 32/*params.icacheBlockSize*/ << endl;
  cout << setw(4)<<"" << "L2-cache size = " << 32768/*params.l2cacheSize*/ << " : ways = " << 1/*params.l2cacheWays*/ << " : block size = " << 64/*params.l2cacheBlockSize*/ << endl;
  cout << setw(4)<<"" << "Memory ready time = " << 50/*params.memoryReadyTime*/ << " : chunksize = " << 8/*params.chunkSize*/ << " : chunktime = " << 15/*params.chunktime*/ << endl;
  cout << endl;

  cout << setw(2)<<"" << "Execute time = " << setw(12) << 678/*data.executeTime*/ << ";  Total refs = " << 10/*data.totalRefs*/ << endl;
}

/*
  Execute time =          678;  Total refs = 10
  Inst refs = 6;  Data refs = 4

  Number of reference types:  [Percentage]
    Reads  =           0    [ 0.0%]
    Writes =           4    [40.0%]
    Inst.  =           6    [60.0%]
    Total  =          10

  Total cycles for activities:  [Percentage]
    Reads  =           0    [ 0.0%]
    Writes =         227    [33.5%]
    Inst.  =         451    [66.5%]
    Total  =         678

  CPI = 113.0
  Ideal: Exec. Time = 16; CPI =  2.7
  Ideal mis-aligned: Exec. Time = 21; CPI =  3.5

  Memory Level:  L1i
    Hit Count = 5  Miss Count = 2
    Total Requests = 7
    Hit Rate = 71.4%   Miss Rate = 28.6%
    Kickouts = 0; Dirty kickouts = 0; Transfers = 2
    VC Hit count = 0

  Memory Level:  L1d
    Hit Count = 7  Miss Count = 1
    Total Requests = 8
    Hit Rate = 87.5%   Miss Rate = 12.5%
    Kickouts = 0; Dirty kickouts = 0; Transfers = 1
    VC Hit count = 0

  Memory Level:  L2
    Hit Count = 0  Miss Count = 3
    Total Requests = 3
    Hit Rate =  0.0%   Miss Rate = 100.0%
    Kickouts = 0; Dirty kickouts = 0; Transfers = 3
    VC Hit count = 0
*/
