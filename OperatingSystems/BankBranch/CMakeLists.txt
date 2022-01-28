cmake_minimum_required(VERSION 3.16)
project(BankBranch C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
set(CMAKE_USE_PTHREADS_INIT true)
set(CMAKE_USE_PTHREADS_INIT ON)
find_package(Threads REQUIRED)



add_executable(BankBranch
        main.c auxiliary.c auxiliary.h)
target_link_libraries(BankBranch Threads::Threads m)
