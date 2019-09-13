cmake_minimum_required(VERSION 3.15)

include(include.cmake)

install(
  TARGETS bound_lifetimes
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  INCLUDES DESTINATION include
)
