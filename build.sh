
#
# build ViennaMaterials
#
mkdir -p build
cd build/
cmake ..
make MAKE="make -j"
make test
