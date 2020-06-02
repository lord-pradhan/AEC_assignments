if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(ps7 MACOSX_BUNDLE main.cpp ps7lib.h ps7lib.cpp)
target_link_libraries(ps7 fslazywindow geblkernel meshlattice glutil)