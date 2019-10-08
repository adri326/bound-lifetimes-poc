cmake_minimum_required(VERSION 3.15)
project(examples_apples LANGUAGES C)


include(include.cmake)

install(
  TARGETS bound_lifetimes
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include
)

add_executable(examples_apples examples/apples/main.c)
target_link_libraries(examples_apples bound_lifetimes)
