cmake_minimum_required(VERSION 3.16.3)
project(testHOG)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_OUTPUT_PATH ${testHOG_SOURCE_DIR}/bin)

include_directories(./include)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -DDEBUG")

add_executable(AHO ./src/HOG-SSP.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(AHO PUBLIC CONSTRUCT_AHO)
add_executable(EHOG ./src/HOG-SSP.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(EHOG PUBLIC CONSTRUCT_EHOG)

add_executable(SSP ./src/HOG-SSP.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(SSP PUBLIC SSP)
add_executable(SK ./src/HOG-SK.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(SK PUBLIC SK)
add_executable(SP ./src/HOG-SP.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(SP PUBLIC SP)
add_executable(SSPEHOG ./src/HOG-SSP.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(SSPEHOG PUBLIC SSP VIA_EHOG)
add_executable(SKEHOG ./src/HOG-SK.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(SKEHOG PUBLIC SK VIA_EHOG)
add_executable(SPEHOG ./src/HOG-SP.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(SPEHOG PUBLIC SP VIA_EHOG)

add_executable(BCER ./src/HOG-BCER.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(BCER PUBLIC BCER)
add_executable(EC ./src/HOG-EC.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(EC PUBLIC EC)
add_executable(BCEREHOG ./src/HOG-BCER.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(BCEREHOG PUBLIC BCER VIA_EHOG)
add_executable(ECEHOG ./src/HOG-EC.cpp ./src/Aho-Cora.cpp ./src/EHOG.cpp ./tests/testHOG.cpp ./src/timer.cpp)
target_compile_definitions(ECEHOG PUBLIC EC VIA_EHOG)

