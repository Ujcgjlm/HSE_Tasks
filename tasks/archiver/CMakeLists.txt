cmake_minimum_required(VERSION 3.16)
project(archiver)

set(CMAKE_CXX_STANDARD 20)
set(OPTIM "-O3 -march=native")
set(DEFFLG "-pthread")
# set(DEBFLG "${DEFFLG} -Wall -Wextra -Werror -Wunused -Wunused-value")
# set(SANIT "-fsanitize=address -static-libasan -fsanitize=leak -fsanitize=undefined -fno-omit-frame-pointer")

set(ALLDEF "${OPTIM} ${DEFFLG}")
set(ALLDEB "${ALLDEF} ${DEBFLG}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ALLDEB} ${SANIT} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(library)
add_subdirectory(tests)

add_executable(archiver
        main.cpp)

target_link_libraries(archiver archiver_library)