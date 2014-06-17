ViennaMaterials
--------------------------

This is the developer repository of ViennaMaterials, being a C++-based
library for providing material parameters to scientific simulation tools via a
convenient API. The default database kernel is based on the pugixml library,
providing XML and XPATH support, which is used by ViennaMaterials' API.
A Python interface is provided via the SWIG library.
ViennaMaterials is currently in a prototype state.


System requirements
--------------------------

* C++ compiler (tested with GCC 4.8.1)
* CMake (tested with CMake 2.8.11.2)
* SWIG (tested with 2.0.10) :: optional, required for Python interface
* libpython (tested with 2.7.5) :: optional, required for Python interface
* UDUNITS (tested with 2.1.23) :: optional, required for physical unit conversions

These packages should be available via your distribution's packaging system,
for instance, on Linux Mint 16 install the following packages: g++, cmake, swig, libpython-dev, libudunits2-dev.

Currently supported operating systems
--------------------------

* GNU/Linux

Building instructions
--------------------------

To build ViennaMaterials, clone the repository and issue the following suggested commands (the following steps are for Unix-based systems):

<pre>
$> cd viennamaterials-dev  # the checked-out GIT folder
$> mkdir build             # the build folder
</pre>

Configure the build, default build type is the 'optimized/release' mode:
<pre>
$> cd build/
$> cmake ..
</pre>

or use the following additional, optional configuration parameters
<pre>
# configure the installation path (default: /usr/local/)
 -DCMAKE_INSTALL_PREFIX=/path/to/install/folder/
# build with debug symbols (default: Release)
 -DCMAKE_BUILD_TYPE=Debug
# build a shared ViennaMaterials library (default: off)
 -DBUILD_SHARED_LIBS=ON
# don't build example applications (default: on)
 -DBUILD_EXAMPLES=OFF
# don't build tests (default: on)
 -DBUILD_TESTS=OFF
# build Python library (default: off)
 -DBUILD_PYLIB=ON
</pre>

For instance, here is a configuration for building the default setup plus the Python module:
<pre>
cmake -DBUILD_PYLIB=ON  ..
</pre>

Watch for errors during CMake's configuration phase, particularly whether CMake was able to locate SWIG or UDUNITS.

Now build the library (and potentially the examples and tests)
<pre>
$> make -j4  # adjust to your CPU core count for efficient parallel building
</pre>

If desired, the required files can be installed to a specific location via issuing.
Note that the run-time path to the ViennaMaterials library will be encoded into the examples,
enabling to execute the examples outside of the build location.
<pre>
make install
</pre>

Examples
--------------------------

If the examples have been activated during the configuration phase, execute the examples in the tutorial build folder
<pre>
$> cd examples/tutorial/
</pre>

Tests
--------------------------

If ViennaMaterials has been configured to generate tests (cf. Building instructions),
you can quickly execute all tests after the building step via executing:
<pre>
$> make test
</pre>
