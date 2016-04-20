#include <iostream>
#include <iomanip>

#ifndef CONFIG_H
#define CONFIG_H
#include "config.h"
#endif

#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "simulator.h"
#endif

#ifndef CACHE_H
#define CACHE_H
#include "cache.h"
#endif

void printCache(Cache * currentCache, std::string cacheName, unsigned int vcSize);

void output(Config params, Data data);
