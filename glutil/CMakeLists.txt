if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(glutil glutil.h glutil.cpp)
target_link_libraries(glutil ysclass)
target_include_directories(glutil PUBLIC .)