cmake_minimum_required(VERSION 3.0)

project(iad)

include_directories(inc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# IAD server build
add_compile_options(-DISSRV)

# call cmake -DDEVEL .
# for compile with traces
if(${DEVEL})
     add_compile_options(-DDEVEL -DXDEVEL)
endif()

# strip binary at link time
add_link_options(-s)

# build from all cpp files in src folder
file(GLOB srv_srcs ./src/**.cpp)
add_executable(iadsrv ${srv_srcs})

target_link_libraries(iadsrv sqlite3 z)
