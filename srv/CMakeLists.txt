cmake_minimum_required(VERSION 2.6)

project(iad)

# find_package(SQLite3 CONFIG REQUIRED)

include_directories(inc)
include_directories(/usr/include)
link_directories(/usr/lib)

file(GLOB srv_srcs ./src/**.cpp)

add_compile_options(-DISSRV)

add_executable(iadsrv ${srv_srcs})

target_link_libraries(iadsrv sqlite3 z)
