# GL functions
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gl.hpp
    COMMAND python3 parser-gl.py gl-func.txt ${CMAKE_CURRENT_SOURCE_DIR}/gl.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/gl.hpp
    DEPENDS ${PROJECT_SOURCE_DIR}/tools/parser-gl.py ${PROJECT_SOURCE_DIR}/tools/gl-func.txt ${CMAKE_CURRENT_SOURCE_DIR}/gl.hpp.in
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tools
    VERBATIM
)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gl.cpp
    COMMAND python3 parser-gl.py gl-func.txt ${CMAKE_CURRENT_SOURCE_DIR}/gl.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/gl.cpp
    DEPENDS ${PROJECT_SOURCE_DIR}/tools/parser-gl.py ${PROJECT_SOURCE_DIR}/tools/gl-func.txt ${CMAKE_CURRENT_SOURCE_DIR}/gl.cpp.in
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tools
    VERBATIM
)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/context.cpp
    COMMAND python3 parser-gl.py gl-func.txt ${CMAKE_CURRENT_SOURCE_DIR}/context.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/context.cpp
    DEPENDS ${PROJECT_SOURCE_DIR}/tools/parser-gl.py ${PROJECT_SOURCE_DIR}/tools/gl-func.txt ${CMAKE_CURRENT_SOURCE_DIR}/context.cpp.in
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tools
    VERBATIM
)

add_library(opengl OBJECT
    context.hpp
    context.cpp
    gl.hpp
    gl.cpp
    loader.hpp
    loader.cpp
    utilities.hpp
    utilities.cpp)
target_include_directories(opengl
    PRIVATE ${PROJECT_BINARY_DIR}/src
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)
target_compile_features(opengl PRIVATE cxx_std_17)