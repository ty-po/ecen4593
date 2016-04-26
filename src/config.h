#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

struct Config {
  std::string traceFile;
  std::string setupName;

  unsigned int icacheSize;
  unsigned int icacheWays;
  unsigned int icacheBlockSize;

  unsigned int dcacheSize;
  unsigned int dcacheWays;
  unsigned int dcacheBlockSize;
  
  unsigned int l1hitTime;
  unsigned int l1missTime;
  unsigned int l1busWidth;

  unsigned int l2cacheSize;
  unsigned int l2cacheWays;
  unsigned int l2cacheBlockSize;

  unsigned int l2hitTime;
  unsigned int l2missTime;
  unsigned int l2transferTime;
  unsigned int l2busWidth;

  unsigned int memorySendAddressTime;
  unsigned int memoryReadyTime;
  unsigned int chunkSize;
  unsigned int chunkTime;

  unsigned int vcSize;
  unsigned int addressBits;

  unsigned int l1iCost;
  unsigned int l1dCost;
  unsigned int l2Cost;
  unsigned int memoryCost;
};

Config defaultConfig();

Config newConfig(int argc, char ** argv);
