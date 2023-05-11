cmake_minimum_required(VERSION 2.6)

project(iad)

# find_package(SQLite3 CONFIG REQUIRED)

include_directories(inc)
include_directories(/usr/include)
link_directories(/usr/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

file(GLOB srv_srcs ./src/**.cpp)

add_compile_options(-DISSRV)
if(${DEVEL})
     add_compile_options(-DDEVEL -DXDEVEL)
endif()

add_executable(iadsrv ${srv_srcs})

target_link_libraries(iadsrv sqlite3 z)
