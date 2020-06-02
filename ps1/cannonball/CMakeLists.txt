if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(cannonball MACOSX_BUNDLE cannonball.cpp)
target_link_libraries(cannonball simplewindow)