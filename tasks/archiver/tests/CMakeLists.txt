cmake_minimum_required(VERSION 3.16)
project(test)

add_executable(tests Treap/TreapUnitTest.cpp Bor/BorUnitTest.cpp BitHandler/BitHandlerUnitTest.cpp)
add_executable(stress_tests Treap/TreapStressTest.cpp Bor/BorStressTest.cpp BitHandler/BitHandlerStressTest.cpp)

target_link_libraries(tests archiver_library)
target_link_libraries(stress_tests archiver_library)