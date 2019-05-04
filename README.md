Protocol Performance Testbench
==============================

Compare multiple communication protocols for
specific characteristics. 

Building
--------

    git clone git@github.com:maximaximal/protocol-testbench.git
	cd protocol-testbench
	mkdir build
	cd build
	cmake .. -DCMAKE_BUILD_TYPE=Release
	make

Depends on the Boost library `program_options`!

Running
-------

Execute `./tester/tester`. A single run may take a while. Sample
simulation results are included in the folder `simulations`. 

Visualising Results
-------------------

To visualise results, you can use the `visualiser.py` script included
in the repository. It generates graphs from the sample simulations.

The visualiser can also take a folder as simulation output source as 
first parameter. For example, to simulate new results from a build
folder, run `python visualiser.py build`.
