#define DEBUG

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif


#include "config.h"
#include "simulator.h"
#include "output.h"


int main(int argc, char ** argv) {
  
  Config(argc, argv);


  output(1);
  return 0;
}
