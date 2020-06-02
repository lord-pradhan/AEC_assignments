if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_library(polygonalmesh polygonalmesh.h polygonalmesh.cpp polygonalmeshio.cpp)
target_link_libraries(polygonalmesh ysclass)
target_include_directories(polygonalmesh PUBLIC .)