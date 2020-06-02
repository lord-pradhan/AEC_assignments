if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


add_executable(ps4_1 main.cpp)
target_link_libraries(ps4_1 simplebitmap hash_util)