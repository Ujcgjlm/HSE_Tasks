cmake_minimum_required(VERSION 3.16)

set(SRC_FILES Treap/Treap.h
        Bor/Bor.cpp
        BitHandler/BitHandler.cpp
        Haffman/Haffman.cpp Bor/BorExceptions.h BitHandler/BitHandlerExceptions.h)

add_library(archiver_library ${SRC_FILES})
target_include_directories(archiver_library PUBLIC ./)