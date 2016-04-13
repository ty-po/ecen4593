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

  cout << setw(2)<<"" << "Execute time = " << right << setw(12) << 678/*data.executeTime*/ << ";  Total refs = " << 10/*data.totalRefs*/ << endl;
  cout << setw(2)<<"" << "Inst refs = " << 6/*data.instructionRefs*/ << ";   Data refs = " << 4/*data.dataRefs*/ << endl;
  cout <<endl;

  cout << setw(2)<<"" << "Number of reference types:  [Percentage]" << endl;
  cout << setw(4)<<"" << "Reads  = " << right << setw(11) << 0/*data.readRefs*/ << "    [" << setw(4) << fixed << setprecision(1) << (float)0/10 *100.0/*data.readRefs/data.totalRefs*/ << "%]" << endl;
  cout << setw(4)<<"" << "Writes = " << right << setw(11) << 4/*data.writeRefs*/ << "    [" << setw(4) << fixed << setprecision(1) << (float)4/10 *100.0/*data.writeRefs/data.totalRefs*/ << "%]" << endl;
  cout << setw(4)<<"" << "Inst.  = " << right << setw(11) << 6/*data.instructionRefs*/ << "    [" << setw(4) << fixed << setprecision(1) << (float)6/10 *100.0/*data.writeRefs/data.totalRefs*/ << "%]" << endl;
  cout << setw(4)<<"" << "Total  = " << right << setw(11) << 10/*data.instructionRefs+readRefs+writeRefs*/ << endl;
  cout<<endl;

  cout << setw(2)<<"" << "Total cycles for activities:  [Percentage]" << endl;
  cout << setw(4)<<"" << "Reads  = " << right << setw(11) << 0/*data.readCycles*/ << "    [" << setw(4) << fixed << setprecision(1) << (float)0/678 *100.0/*data.readCycles/data.totalCycles*/ << "%]" << endl;
  cout << setw(4)<<"" << "Writes = " << right << setw(11) << 227/*data.writeCycles*/ << "    [" << setw(4) << fixed << setprecision(1) << (float)227/678 *100.0/*data.writeCycles/data.totalCycles*/ << "%]" << endl;
  cout << setw(4)<<"" << "Inst.  = " << right << setw(11) << 451/*data.instructionCycles*/ << "    [" << setw(4) << fixed << setprecision(1) << (float)451/678 *100.0/*data.writeCycles/data.totalCycles*/ << "%]" << endl;
  cout << setw(4)<<"" << "Total  = " << right << setw(11) << 678/*data.instructionCycles+readCycles+writeCycles*/ << endl;
  cout<<endl;

  cout << setw(2)<<"" << "CPI = " << (float)678/6 /*data.totalCycles/data.instructionRefs*/ << endl;
  
}

/*
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
