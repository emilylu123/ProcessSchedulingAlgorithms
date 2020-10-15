cmake_minimum_required(VERSION 3.16)
project(assignment1)

set(CMAKE_CXX_STANDARD 14)

include_directories(test_data)

add_executable(assignment1
        test_data/case0/input.txt
        test_data/case0/output.txt
        test_data/case0/output_detail.txt
        test_data/case1/input.txt
        test_data/case1/output.txt
        test_data/case1/output_detail.txt
        test_data/case2/input.txt
        test_data/case2/output.txt
        test_data/case2/output_detail.txt
        test_data/readme.txt
        TicketBooker.cpp)
