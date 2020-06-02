if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(ps6_1 MACOSX_BUNDLE main.cpp)
target_link_libraries(ps6_1 fslazywindow naca4digit polygonalmesh)