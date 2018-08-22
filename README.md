This is an unofficial fork of the original [mining simulator](https://github.com/citp/mining_simulator)
by Harry Kallodner. The original project is still available for development and we encourage you to check
it out before looking at this one as both have their niche.

Bitcoin PayForward Mining Simulator 
=======

This is a simulator intended for testing non-default bitcoin mining strategies.

Installation
------------

The Bitcoin Mining Simulator has been developed and tested on Mac OS X 10.13.3.
This software is currently setup to be run from the command line.
It has a single dependency: (https://www.gnu.org/software/gsl/).
The code is written in C++14 and a Makefile is included.

```
make test
make run
```

Currently only the learning model is tested

Architecture
------------
The code is divided into the ```main.cpp``` and the ```src``` modules: 
the ```learning_model```, ```strategy_behaviour``` and ```mining_game```.
The first implements the algorithm that player use to choose strategies, the 
second describes the strategies used for mining and the third describes the 
blockchain as a cohesive unit of blocks, miners and a blockchain data structure.

Usage
-----------

The code is generally designed around the idea of mining strategies. Each
mining strategy prescribes set behaviors for a mining. We then create a set of
miners with configurable hash rate, cost per hash, and network latency. These
miners then compete to mine on a simulated blockchain with configurable block
reward and transaction fee accumulation rate. More documentation to follow.

Parameters are configurable via a ```config.json``` or via runtime parameters.
To see them, enter:
```
./run -h
```

To get started, try modifying the ```config.json``` or running ```./run -c -n 1```
to see the commentary of a single game.

Output of Simulator
------------------------
By default, the simulations produce a folder named ```results``` with a file for each strategy. Each
output file gives a list of the percentage of miners who used the given strategy
in each round. This demonstrates the idea that miners will either all converge to
an optimal strategy or oscillate between strategies when there is no equilibrium.

These can be visualised using a the python script provided in this project
'''
python visualizer/visualize.py -f results
'''

Disclaimer
-----------

Note that the Bitcoin Mining Simulator is under active development, and should
be considered experimental software. We plan on writing comprehensive tests to 
verify the results of all included components. Prior to using the Bitcoin Mining
Simulator for your own research we encourage you to write tests (and submit pull 
requests!) for any testing that isn't currently included in our test scripts.

License
-------

Bitcoin PayForward Mining Simulator is licensed under GNU GPLv3. Additional code has been included from
[arithmetic_type](https://github.com/gnzlbg/arithmetic_type) which is licensed Boost Software License Version 1.0.