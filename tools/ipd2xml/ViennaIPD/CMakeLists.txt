CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

PROJECT(viennaipd C CXX)

#### Build type. 
#### Supported values: None, Debug, Release, RelWithDebInfo, MinSizeRel
SET(CMAKE_BUILD_TYPE "Release")

#### Build shared library (call 'cmake .. -DBUILD_SHARED_LIBS=OFF' in build-directory to build a static library)
OPTION(BUILD_SHARED_LIBS "Build shared (.DLL .SO) library." ON)

#### Include directories
INCLUDE_DIRECTORIES(
        ./src/include
	./src/core/include
    )

#### Source directories
aux_source_directory("./src" IPD_SRC)
aux_source_directory("./src/core/error" CORE_ERROR_SRC)
aux_source_directory("./src/core/sys" CORE_SYS_SRC)
aux_source_directory("./src/core/unit" CORE_UNIT_SRC)

#### Build target
ADD_LIBRARY (viennaipd ${IPD_SRC} ${CORE_ERROR_SRC} ${CORE_SYS_SRC} ${CORE_UNIT_SRC})

