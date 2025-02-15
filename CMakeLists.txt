cmake_minimum_required(VERSION 3.29)
project(ParallelTimedFifo)

set(CMAKE_CXX_STANDARD 11)

add_executable(ParallelTimedFifo main.cpp
        ParallelTimedFifo.h
        StableTimedFifo.h)
