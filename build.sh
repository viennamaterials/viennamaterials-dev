# build the doxygen documentation
cd doc/
doxygen
cd .. 

# build the library and the tests
mkdir -p build
cd build/
cmake ..
make MAKE="make -j"
make test
