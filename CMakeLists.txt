cmake_minimum_required(VERSION 3.12)
project(stackoverflow_in_cpp3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(hdr)
include_directories(hdr/sqlite_modern_cpp)
include_directories(hdr/sqlite_modern_cpp/lists)
include_directories(hdr/sqlite_modern_cpp/utility)

add_executable(stackoverflow_in_cpp3
        hdr/sqlite_modern_cpp/lists/error_codes.h
        hdr/sqlite_modern_cpp/utility/function_traits.h
        hdr/sqlite_modern_cpp/utility/uncaught_exceptions.h
        hdr/sqlite_modern_cpp/utility/utf16_utf8.h
        hdr/sqlite_modern_cpp/utility/variant.h
        hdr/sqlite_modern_cpp/errors.h
        hdr/sqlite_modern_cpp/log.h
        hdr/sqlite_modern_cpp/sqlcipher.h
        hdr/sqlite_modern_cpp.h
        AbstractUser.h
        Content.cpp
        Content.h
        Database.cpp
        Database.h
        Design.png
        Exceptions.h
        LICENSE
        Logger.cpp
        Logger.h
        main.cpp
        README.md
        User.cpp
        User.h)

target_link_libraries(stackoverflow_in_cpp3 LINK_PUBLIC sqlite3)