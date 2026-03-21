cmake_minimum_required(VERSION 3.20)

project(LogParserProject LANGUAGES CXX)

option(BUILD_TESTS "Build unit tests" ON)

# ---- C++ standard ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Core library (your production code) ----
add_library(logparser_core
    Data_NS/DataStructure.cpp
    Parser_NS/LogParser.cpp
    Parser_NS/LogParserFuture.cpp
    Parser_NS/LogParserThreads.cpp
    Parser_NS/LogParserCreaterConc.cpp
    Parser_NS/LogParserCreaterLazy.cpp
)

target_include_directories(logparser_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Threads (needed on Linux; harmless on Windows)
find_package(Threads REQUIRED)
target_link_libraries(logparser_core PUBLIC Threads::Threads)

# ---- Main app ----
add_executable(logparser_app
    main.cpp
)

target_link_libraries(logparser_app PRIVATE logparser_core)

# ---- Warnings / flags ----
if (MSVC)
    target_compile_options(logparser_core PRIVATE /W4 /permissive- /EHsc)
    target_compile_options(logparser_app  PRIVATE /W4 /permissive- /EHsc)
else()
    target_compile_options(logparser_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(logparser_app  PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---- Tests ----
if (BUILD_TESTS)
    enable_testing()

    add_executable(unit_tests
        Test_NS/test.cpp
        Test_NS/CreateLargeLogs.cpp
        Test_NS/catch_amalgamated.cpp
    )

    target_include_directories(unit_tests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/Test_NS
    )

    target_link_libraries(unit_tests PRIVATE logparser_core Threads::Threads)

    if (MSVC)
        target_compile_options(unit_tests PRIVATE /W4 /permissive- /EHsc)
    else()
        target_compile_options(unit_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    add_test(NAME unit_tests COMMAND unit_tests)
endif()
