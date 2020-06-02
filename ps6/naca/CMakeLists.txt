if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(naca4digit naca4digit.h naca4digit.cpp)
target_link_libraries(naca4digit ysclass polygonalmesh)
target_include_directories(naca4digit PUBLIC .)