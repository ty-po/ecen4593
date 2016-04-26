#include "config.h" 
using namespace std;



Config newConfig(int argc, char ** argv){
  Config params = defaultConfig();

  if(argc >= 3) {
    params.traceFile = argv[2];
  }



  if(argc >= 2) {

    params.setupName = argv[1];
    string path = "../test/config/";
    path.append(argv[1]);
    ifstream configFile (path);

    string param = "";
    unsigned int value = 0;

    string line;

    if(configFile.is_open()) {
      while(getline(configFile, line)) {
      
        istringstream buffer(line);

        buffer >> param;
        buffer >> value;
        if(value && param != "") {
          if(param == "icacheSize") params.icacheSize = value;
          if(param == "icacheWays") params.icacheWays = value;
          if(param == "icacheBlockSize") params.icacheBlockSize = value;

          if(param == "dcacheSize") params.dcacheSize = value;
          if(param == "dcacheWays") params.dcacheWays = value;
          if(param == "dcacheBlockSize") params.dcacheBlockSize = value;
          
          if(param == "l1hitTime") params.l1hitTime = value;
          if(param == "l1missTime") params.l1missTime = value;
          if(param == "l1busWidth") params.l1busWidth = value;

          if(param == "l2cacheSize") params.l2cacheSize = value;
          if(param == "l2cacheWays") params.l2cacheWays = value;
          if(param == "l2cacheBlockSize") params.l2cacheBlockSize = value;
          
          if(param == "l2hitTime") params.l2hitTime = value;
          if(param == "l2missTime") params.l2missTime = value;
          if(param == "l2transferTime") params.l2transferTime = value;
          if(param == "l2busWidth") params.l2busWidth = value;

          if(param == "memorySendAddressTime") params.memorySendAddressTime = value;
          if(param == "memoryReadyTime") params.memoryReadyTime = value;
          if(param == "chunkSize") params.chunkSize = value;
          if(param == "chunkTime") params.chunkTime = value;

          if(param == "vcSize") params.vcSize = value;
          if(param == "addressBits") params.addressBits = value;
        }

        param = "";
        value = 0;
        
      }
      configFile.close();
    }
    else cout << "failed to open" << endl;
  }
  params.l1iCost = ((params.icacheSize*100)/4096)*(1 + (params.icacheWays/2));
  params.l1dCost = ((params.dcacheSize*100)/4096)*(1 + (params.dcacheWays/2)); 
  params.l2Cost = ((params.l2cacheSize*50)/16384)*(1 + (params.l2cacheWays/2)); 
  params.memoryCost = 75 + 200*((50/params.memoryReadyTime) - 1) + 100*((params.chunkSize/8) - 1) ;


  return params;
}

Config defaultConfig() {
  
  Config d;

  d.traceFile = "trX";
  d.setupName = "default";

  d.icacheSize = 8192;
  d.icacheWays = 1;
  d.icacheBlockSize = 32;

  d.dcacheSize = 8192;
  d.dcacheWays = 1;
  d.dcacheBlockSize = 32;

  d.l1hitTime = 1;
  d.l1missTime = 1;
  d.l1busWidth = 4;

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

  d.vcSize = 8;
  d.addressBits = 64;

  d.l1iCost = 200;
  d.l1dCost = 200;
  d.l2Cost = 100;
  d.memoryCost = 75;

  return d;
}
