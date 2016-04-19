#include "config.h" 

Config defaultConfig() {
  
  Config d;

  d.traceFile = "trX";
  d.setupName = "default";

  d.dcacheSize = 8192;
  d.dcacheWays = 1;
  d.dcacheBlockSize = 32;

  d.icacheSize = 8192;
  d.icacheWays = 1;
  d.icacheBlockSize = 32;

  d.l1hitTime = 1;
  d.l1missTime = 1;

  d.l2cacheSize = 32768;
  d.l2cacheWays = 1;
  d.l2cacheBlockSize = 64;

  d.l2hitTime = 8;
  d.l2missTime = 10;
  d.l2transferTime = 10;
  d.l2busWidth = 16;

  d.memorySendAddressTime = 10;
  d.memoryReadyTime = 50;
  d.chunkSize = 8;
  d.chunkTime = 15;

  d.l1iCost = 200;
  d.l1dCost = 200;
  d.l2Cost = 100;
  d.memoryCost = 75;

  return d;
}

Config newConfig(int argc, char ** argv){
  
  Config params = defaultConfig();
  
  

  return params;
}
