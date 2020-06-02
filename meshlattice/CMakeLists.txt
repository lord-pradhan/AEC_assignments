if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(meshlattice meshlattice.h meshlattice.cpp)
target_link_libraries(meshlattice ysclass geblkernel)
target_include_directories(meshlattice PUBLIC .)