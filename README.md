ViennaMaterials 
--------------------------

This is the developer repository of ViennaMaterials, being a C++-based 
library for providing material parameters to scientific simulation tools via a 
convenient API. The default database kernel is based on the pugixml library, 
providing XML and XPATH support, which is used by ViennaMaterials' API.
ViennaMaterials is currently in a prototype state. 


System requirements
--------------------------

* C++ compiler
* [libxml2](http://xmlsoft.org) (optional, for DTD checks of the input XML data)
* [Boost](http://www.boost.org/) (optional, for Serialization support)

Currently Supported operating systems
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
$> cmake ..  
</pre>

or use the following additional and optional configuration parameters
<pre>
$> cmake -DCMAKE_BUILD_TYPE=DEBUG ..    # build with debug symbols (default: off)
$> cmake -DBUILD_SHARED_LIBS=ON         # build a shared ViennaMaterials library (default: off)
$> cmake -DBUILD_EXAMPLES=OFF           # don't build example applications (default: on)
$> cmake -DBUILD_DTD_CHECK_SUPPORT=OFF  # don't include DTD checks, which requires libxml2 library (default: off)
$> cmake -DBUILD_SERIALIZATION=OFF      # don't include Serialization support, which requires Boost.Serialization (default: off)
</pre>

Now build and install the executable and libraries (install folder is a subfolder of the build folder):
<pre>
$> make -j4  # adjust to your CPU core count for efficient parallel building
$> make install
</pre>
