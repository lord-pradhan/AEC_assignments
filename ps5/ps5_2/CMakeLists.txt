if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(ps5_2 MACOSX_BUNDLE main.cpp)
target_link_libraries(ps5_2 fslazywindow ysbitmapfont bintreeutil)