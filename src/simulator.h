#ifndef CONFIG_H
#define CONFIG_H
#include "config.h"
#endif


class Data {
  public:
    int Reads;
    int Writes;
    int Instructions;
};


Data simulator(Config params);
