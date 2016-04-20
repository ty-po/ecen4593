#ifndef CONFIG_H
#define CONFIG_H
#include "config.h"
#endif

#ifndef CACHE_H
#define CACHE_H
#include "cache.h"
#endif

struct Data {

  unsigned long long int executeTime;

/*Refs*/
  unsigned long int totalRefs;

  unsigned long int readRefs;
  unsigned long int writeRefs;
  unsigned long int instructionRefs;

/*Cycles*/
  unsigned long long int totalCycles;

  unsigned long long int readCycles; 
  unsigned long long int writeCycles; 
  unsigned long long int instructionCycles;

/*Ideal*/
  unsigned long long int idealTime;
  unsigned long long int idealMisAlignedTime;

 /*L1 Icache*/
  Cache * L1i;

  unsigned long int l1iTotalRequests;

  unsigned long int l1iHitCount;
  unsigned long int l1iMissCount;

  unsigned long int l1iKickouts;
  unsigned long int l1iDirtyKickouts;
  unsigned long int l1iTransfers;
  unsigned long int l1iVCHitCount;

 /*L1 Dcache*/ 
  Cache * L1d;

  unsigned long int l1dTotalRequests;

  unsigned long int l1dHitCount;
  unsigned long int l1dMissCount;

  unsigned long int l1dKickouts;
  unsigned long int l1dDirtyKickouts;
  unsigned long int l1dTransfers;
  unsigned long int l1dVCHitCount;

 /*L2 cache*/ 
  Cache * L2;

  unsigned long int l2TotalRequests;

  unsigned long int l2HitCount;
  unsigned long int l2MissCount;

  unsigned long int l2Kickouts;
  unsigned long int l2DirtyKickouts;
  unsigned long int l2Transfers;
  unsigned long int l2VCHitCount;
};

Data simulator(Config params);
