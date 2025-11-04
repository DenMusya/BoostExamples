cmake_minimum_required(VERSION 4.1)
cmake_policy(SET CMP0167 NEW)

find_package(Boost 1.89 REQUIRED)

add_subdirectory(00_test)
add_subdirectory(01_SyncTimer)
add_subdirectory(02_AsyncTimer)
add_subdirectory(03_ArgumentsBinding)
add_subdirectory(04_MemberFunctionAsCompletionHandler)
add_subdirectory(05_Multithreading)