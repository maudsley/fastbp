cmake_minimum_required(VERSION 3.0)

# where is boost?
include_directories(/opt/boost)
link_directories(/opt/boost/stage/lib)

# where is gtest?
include_directories(/opt/googletest/googletest/include)
link_directories(/opt/googletest/googlemock/gtest)

# enable C++11 and warnings
set(CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")

include_directories(.)
include_directories(./src)

set(AppSources
  main.cpp
  matrix_util.cpp
)

set(AppLibraries
)

set(AppLibraries
)

add_executable(fastbp ${AppSources})
target_link_libraries(fastbp ${AppLibraries})
