cmake_minimum_required(VERSION 3.2)
project(api)

# Add executable
add_executable(local_${PROJECT_NAME}_test 
  local_api_test.c
)
add_executable(remote_${PROJECT_NAME}_test 
  remote_api_test.c
)

# Add lib
add_library(libcomm
  curi_communication.c
  curi_communication.h
)

#Link lib
target_link_libraries(local_${PROJECT_NAME}_test 
  PRIVATE libcomm
)

target_link_libraries(remote_${PROJECT_NAME}_test 
  PRIVATE libcomm
)