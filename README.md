ViennaMaterials
--------------------------

This is the developer repository of ViennaMaterials, being a C++-based
library for providing material parameters to scientific simulation tools via a
convenient API. The default database kernel is based on the pugixml library,
providing XML and XPATH support, which is used by ViennaMaterials' API.
ViennaMaterials is currently in a prototype state.


System requirements
--------------------------

* C++ compiler (tested on GCC 4.8.1)

Currently supported operating systems
--------------------------

* Linux

Building instructions
--------------------------

To build ViennaMaterials, simply clone the repository and issue the following suggested commands (the following steps are for Unix-based systems):

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
# build with debug symbols (default: off)
 -DCMAKE_BUILD_TYPE=DEBUG
# build a shared ViennaMaterials library (default: on)
 -DBUILD_SHARED_LIBS=OFF
# don't build example applications (default: on)
 -DBUILD_EXAMPLES=OFF
# don't build tests (default: on)
 -DBUILD_TESTS=OFF
# build Python library (default: off)
 -DBUILD_PYLIB=ON
</pre>

Now build the library (and potentially the examples and tests)
<pre>
$> make -j4  # adjust to your CPU core count for efficient parallel building
</pre>

Examples
--------------------------

Execute the examples in the tutorial build folder
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
