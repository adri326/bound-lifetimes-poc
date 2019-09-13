set(
  BOUND_LIFETIMES_SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/bound_lifetimes/runtime.c
  ${CMAKE_CURRENT_LIST_DIR}/bound_lifetimes/runtime.h
)

add_library(bound_lifetimes ${BOUND_LIFETIMES_SOURCES})
include_directories(${CMAKE_CURRENT_LIST_DIR})
