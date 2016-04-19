#define DEBUG

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

#ifndef CONFIG_H
#define CONFIG_H
#include "config.h"
#endif


#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "simulator.h"
#endif

#include "output.h"


int main(int argc, char ** argv) {
 
  Config params = newConfig(argc, argv);

  Data data = simulator(params);

  output(params, data);
  return 0;
}
