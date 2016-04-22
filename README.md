#ECEN 4593 Final Project
Memory Simulation

##Usage
To Run Once

1. in `src/` do `cmake .`, `make`
2. in `bin/` do `cat trX | ./ecen4593final`

To Watch
1. in `test/utils/` do `./watch.sh`
2. or `./watch.sh trX

##Requirements
1. Simulate and Evaluate Multiple Memory Configurations
2. Configurable Memory Configuration
3. Read/Simulate Billion Line Traces
4. Output computed data

##Components
###Memory Config File/Parser/Specification
Read in Memory Config/Compute Simulator Config Inputs
#####Inputs
Command Line Arguments
#####Outputs
Config data type with cache configuration


###Cache Simulator
Body
#####Inputs
Config data type
#####Outputs
Raw data


###Data Output
Print Collected Data
#####Inputs
Raw data
#####Outputs
Printout

##TODO
- [ ] Config File and Parsing
- [ ] Alignment Calculation
- [ ] Tracking in simulator.cc
- [ ] Testing
- [ ] VC Dirty kickout writeback
