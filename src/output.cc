#include "output.h"
using namespace std;

void help() {

}

void output(Config params, Data data) {
  cout << endl;
  cout << "-------------------------------------------------------------------------" << endl;
  cout << setw(6)<<"" << left << setw(3)<< params.traceFile << "." << setw(16)<< params.setupName << "Simulation Results" << endl;
  cout << "-------------------------------------------------------------------------" << endl;
  cout << endl;

  cout << setw(2)<<"" << "Memory system:" << endl;
  cout << setw(4)<<"" << "Dcache size = " << params.dcacheSize << " : ways = " << params.dcacheWays << " : block size = " << params.dcacheBlockSize << endl;
  cout << setw(4)<<"" << "Icache size = " << params.icacheSize << " : ways = " << params.icacheWays << " : block size = " << params.icacheBlockSize << endl;
  cout << setw(4)<<"" << "L2-cache size = " << params.l2cacheSize << " : ways = " << params.l2cacheWays << " : block size = " << params.l2cacheBlockSize << endl;
  cout << setw(4)<<"" << "Memory ready time = " << params.memoryReadyTime << " : chunksize = " << params.chunkSize << " : chunktime = " << params.chunkTime << endl;
  cout << endl;

  cout << setw(2)<<"" << "Execute time = " << right << setw(12) << data.executeTime << ";  Total refs = " << data.totalRefs << endl;
  cout << setw(2)<<"" << "Inst refs = " << data.instructionRefs << ";   Data refs = " << data.readRefs+data.writeRefs << endl;
  cout <<endl;

  cout << setw(2)<<"" << "Number of reference types:  [Percentage]" << endl;
  cout << setw(4)<<"" << "Reads  = " << right << setw(11) << data.readRefs << "    [" << setw(4) << fixed << setprecision(1) << (float)data.readRefs/data.totalRefs * 100 << "%]" << endl;
  cout << setw(4)<<"" << "Writes = " << right << setw(11) << data.writeRefs << "    [" << setw(4) << fixed << setprecision(1) << (float)data.writeRefs/data.totalRefs * 100 << "%]" << endl;
  cout << setw(4)<<"" << "Inst.  = " << right << setw(11) << data.instructionRefs << "    [" << setw(4) << fixed << setprecision(1) << (float)data.instructionRefs/data.totalRefs * 100 << "%]" << endl;
  cout << setw(4)<<"" << "Total  = " << right << setw(11) << data.instructionRefs+data.readRefs+data.writeRefs << endl;
  cout<<endl;

  cout << setw(2)<<"" << "Total cycles for activities:  [Percentage]" << endl;
  cout << setw(4)<<"" << "Reads  = " << right << setw(11) << data.readCycles << "    [" << setw(4) << fixed << setprecision(1) << (float)data.readCycles/data.totalCycles * 100 << "%]" << endl;
  cout << setw(4)<<"" << "Writes = " << right << setw(11) << data.writeCycles << "    [" << setw(4) << fixed << setprecision(1) << (float)data.writeCycles/data.totalCycles * 100 << "%]" << endl;
  cout << setw(4)<<"" << "Inst.  = " << right << setw(11) << data.instructionCycles << "    [" << setw(4) << fixed << setprecision(1) << (float)data.writeCycles/data.totalCycles * 100 << "%]" << endl;
  cout << setw(4)<<"" << "Total  = " << right << setw(11) << data.instructionCycles+data.readCycles+data.writeCycles << endl;
  cout<<endl;

  cout << setw(2)<<"" << "CPI = " << (float)data.totalCycles/data.instructionRefs << endl;
  cout << setw(2)<<"" << "Ideal: Exec. Time = " << data.idealTime << "; CPI = " << (float)data.idealTime/data.instructionRefs << endl;
  cout << setw(2)<<"" << "Ideal mis-aligned: Exec. Time = " << data.idealMisAlignedTime << "; CPI = " << (float)data.idealMisAlignedTime/data.instructionRefs << endl;
  cout << endl;

  cout << setw(2)<<"" << "Memory Level:  L1i" << endl;
  cout << setw(4)<<"" << "Hit Count = " << data.l1iHitCount << "  Miss Count = " << data.l1iMissCount << endl;
  cout << setw(4)<<"" << "Total Requests = " << data.l1iTotalRequests << endl;
  cout << setw(4)<<"" << "Hit Rate = " << (float)data.l1iHitCount/data.l1iTotalRequests * 100 << "%  Miss Rate = " << (float)data.l1iMissCount/data.l1iTotalRequests * 100 << "%" << endl;
  cout << setw(4)<<"" << "Kickouts = " << data.l1iKickouts << "; Dirty kickouts = " << data.l1iDirtyKickouts << "; Transfers = " << data.l1iTransfers << endl;
  cout << setw(4)<<"" << "VC Hit count = " << data.l1iVCHitCount << endl;
  cout << endl;

  cout << setw(2)<<"" << "Memory Level:  L1d" << endl;
  cout << setw(4)<<"" << "Hit Count = " << data.l1dHitCount << "  Miss Count = " << data.l1dMissCount << endl;
  cout << setw(4)<<"" << "Total Requests = " << data.l1dTotalRequests << endl;
  cout << setw(4)<<"" << "Hit Rate = " << (float)data.l1dHitCount/data.l1dTotalRequests * 100 << "%  Miss Rate = " << (float)data.l1dMissCount/data.l1dTotalRequests * 100 << "%" << endl;
  cout << setw(4)<<"" << "Kickouts = " << data.l1dKickouts << "; Dirty kickouts = " << data.l1dDirtyKickouts << "; Transfers = " << data.l1dTransfers << endl;
  cout << setw(4)<<"" << "VC Hit count = " << data.l1dVCHitCount << endl;
  cout << endl;

  cout << setw(2)<<"" << "Memory Level:  L2" << endl;
  cout << setw(4)<<"" << "Hit Count = " << data.l2HitCount << "  Miss Count = " << data.l2MissCount << endl;
  cout << setw(4)<<"" << "Total Requests = " << data.l2TotalRequests << endl;
  cout << setw(4)<<"" << "Hit Rate = " << (float)data.l2HitCount/data.l2TotalRequests * 100 << "%  Miss Rate = " << (float)data.l2MissCount/data.l2TotalRequests * 100 << "%" << endl;
  cout << setw(4)<<"" << "Kickouts = " << data.l2Kickouts << "; Dirty kickouts = " << data.l2DirtyKickouts << "; Transfers = " << data.l2Transfers << endl;
  cout << setw(4)<<"" << "VC Hit count = " << data.l2VCHitCount << endl;
  cout << endl;

  cout << setw(2)<<"" << "L1 cache cost (Icache $" << params.l1iCost << ") + (Dcache $" << params.l1dCost << ") = $" << params.l1iCost+params.l1dCost << endl;
  cout << setw(2)<<"" << "L2 cache cost = $" << params.l2Cost << ";  Memory cost = $" << params.memoryCost << "  Total cost = $" << params.l1iCost+params.l1dCost+params.l2Cost+params.memoryCost<< endl;
  cout << endl;

  cout << "-------------------------------------------------------------------------" << endl;
  cout << endl;

  cout << "Cache final contents - Index and Tag values are in HEX" << endl;
  cout << endl;

  int i;

  /*****    L1i Print    *****/
  cout << "Memory Level: L1i" << endl;
  for(i=0;i<2;i++) {
    cout << "Index: " << setw(4) << 0/*index*/ << " | V:" << 1/*valid*/ << " D:" << 0/*dirty*/ << " Tag: " << setw(12) << "3fc0e7112"/*dirty*/ << " |" << endl;
  }
  cout << "Victim cache:" << endl;
  for(i=0;i<8;i++) {
    if(!(i%2)) {
      cout << setw(12)<<"";
    }

    cout << "| V:" << 0/*VC-v*/ << " D:" << 0/*VC-d*/ << " Addr: " << setw(12) << "-"/*VC-address*/;

    if(i%2) {
      cout << " |" << endl;
    }
  }
  /*****  End L1i Print  *****/

  /*****    L1d Print    *****/
  cout << "Memory Level: L1d" << endl;
  for(i=0;i<2;i++) {
    cout << "Index: " << setw(4) << 0/*index*/ << " | V:" << 1/*valid*/ << " D:" << 0/*dirty*/ << " Tag: " << setw(12) << "3fc0e7112"/*dirty*/ << " |" << endl;
  }
  cout << "Victim cache:" << endl;
  for(i=0;i<8;i++) {
    if(!(i%2)) {
      cout << setw(12)<<"";
    }

    cout << "| V:" << 0/*VC-v*/ << " D:" << 0/*VC-d*/ << " Addr: " << setw(12) << "-"/*VC-address*/;

    if(i%2) {
      cout << " |" << endl;
    }
  }
  /*****  End L1d Print  *****/
  
  /*****    L2  Print    *****/
  cout << "Memory Level: L2" << endl;
  for(i=0;i<2;i++) {
    cout << "Index: " << setw(4) << 0/*index*/ << " | V:" << 1/*valid*/ << " D:" << 0/*dirty*/ << " Tag: " << setw(12) << "3fc0e7112"/*dirty*/ << " |" << endl;
  }
  cout << "Victim cache:" << endl;
  for(i=0;i<8;i++) {
    if(!(i%2)) {
      cout << setw(12)<<"";
    }

    cout << "| V:" << 0/*VC-v*/ << " D:" << 0/*VC-d*/ << " Addr: " << setw(12) << "-"/*VC-address*/;

    if(i%2) {
      cout << " |" << endl;
    }
  }
  /*****  End L2  Print  *****/
}
