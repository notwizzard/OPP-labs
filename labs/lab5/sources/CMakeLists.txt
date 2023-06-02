set(SOURCES sources)
add_library(
        ${SOURCES} STATIC
        Task.hpp Task.cpp ThreadService.hpp ThreadService.cpp)
target_include_directories(${SOURCES} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")