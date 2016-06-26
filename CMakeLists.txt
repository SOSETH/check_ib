cmake_minimum_required(VERSION 3.0)
project(check_ib)

# http://stackoverflow.com/questions/10851247/how-to-activate-c-11-in-cmake
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp IBHost.cpp IBHost.h IBPort.cpp IBPort.h IBPortRegistry.cpp IBPortRegistry.cpp IBPortRegistry.h IBException.h IBNetfileParser.cpp IBNetfileParser.h IBAddress.cpp IBAddress.h IBHostRegistry.cpp IBHostRegistry.h IBValidator.h IcingaOutput.cpp IcingaOutput.h IBHostValidator.cpp IBHostValidator.h IBValidator.cpp IBNetworkValidator.cpp IBNetworkValidator.h)

find_package( Boost 1.50 COMPONENTS program_options REQUIRED )
include_directories(/usr/include/infiniband ${Boost_INCLUDE_DIR})

add_executable(check_ib ${SOURCE_FILES})
target_link_libraries(check_ib ibnetdisc ibmad yaml-cpp ${Boost_LIBRARIES})