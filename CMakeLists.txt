cmake_minimum_required(VERSION 3.20)
project(mylox)

set(CMAKE_CXX_STANDARD 20)


add_executable(mylox main.cpp scanner.cpp scanner.h Log.h expr.cpp expr.h parser.cpp parser.h Printer.cpp Printer.h)


if(MSVC)
    target_compile_options(mylox PRIVATE /W4 /WX)
else()
    target_compile_options(mylox PRIVATE -Wall -Werror -Wextra)
endif()