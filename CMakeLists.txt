cmake_minimum_required(VERSION 3.15)
project(integer_sets)
include_directories(include)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(GMP REQUIRED)
set(CMAKE_CXX_STANDARD 14)

#[[
#############
## RocksDB ##
#############
add_subdirectory(submodules/rocksdb)
include_directories(submodules/rocksdb/include)
find_package(snappy)
include_directories(${SNAPPY_INCLUDE_DIR})
find_package(LZ4)
include_directories(${LZ4_INCLUDE_DIR})
find_package(RocksDB)
message(STATUS "RocksDB: ${ROCKSDB_INCLUDE_DIRS}")
message(STATUS "${ROCKSDB_LIBRARIES}")
include_directories(${ROCKSDB_INCLUDE_DIRS})
#${SNAPPY_LIBRARIES} ${LZ4_LIBRARY} ${ROCKSDB_LIBRARIES}
LINK_DIRECTORIES(${SNAPPY_LIBRARIES})
LINK_DIRECTORIES(${LZ4_LIBRARY})
LINK_DIRECTORIES(${ROCKSDB_LIBRARIES})

set(LIBM m)
set(LIBZ z)
set(LIBZ4 lz4)
set(LIBZSTD zstd)
set(LIBDL dl)
set(LIBSNAPPY snappy)
set(LIBBZ2 bz2)
set(LIBFDB forestdb)
set(LIBLDB leveldb)
set(LIBRDB rocksdb)
]]

add_executable(integer_sets main.cpp src/basic_operations.cpp include/basic_operations.h src/BigIntegerForBitMask.cpp include/BigIntegerForBitMask.h include/block_sm_intgraph.h src/MPZOffsetComparator.cpp include/MPZOffsetComparator.h include/RocksDBKeyValue.h mpzDAG.cpp)
target_link_libraries(integer_sets  ${LIBSNAPPY} ${LIBZ4} ${ROCKSDB_STATIC_LIB})
