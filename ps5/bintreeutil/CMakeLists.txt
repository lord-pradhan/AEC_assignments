if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(bintreeutil bintree.h bintree.cpp)
target_include_directories(bintreeutil PUBLIC .)