#include <string>

struct Config {
  std::string traceFile;
  std::string setupName;

  unsigned int dcacheSize;
  unsigned int dcacheWays;
  unsigned int dcacheBlockSize;
  
  unsigned int icacheSize;
  unsigned int icacheWays;
  unsigned int icacheBlockSize;

  unsigned int l1hitTime;
  unsigned int l1missTime;

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

  unsigned int l1iCost;
  unsigned int l1dCost;
  unsigned int l2Cost;
  unsigned int memoryCost;
};

Config defaultConfig();

Config newConfig(int argc, char ** argv);
