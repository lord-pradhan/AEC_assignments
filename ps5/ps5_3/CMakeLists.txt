if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(ps5_3 main.cpp)

target_link_libraries(ps5_3 bintreeutil)
add_test(test3 ps5_3)