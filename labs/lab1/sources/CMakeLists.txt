set(SOURCES sources)
add_library(
        ${SOURCES} STATIC
        SolutionV2.hpp SolutionV2.cpp AbstractSolution.cpp AbstractSolution.hpp SolutionV1.hpp SolutionV1.cpp CommonSolutionV1.cpp CommonSolutionV1.hpp RootSolutionV1.cpp RootSolutionV1.hpp Utils.cpp Utils.hpp SolutionFactory.hpp SolutionFactory.cpp)
target_include_directories(${SOURCES} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")