add_library(entity OBJECT
    weapon.hpp
    weapon.cpp
)
target_compile_features(entity PRIVATE cxx_std_20)
target_include_directories(entity
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PRIVATE ${PROJECT_BINARY_DIR}/src
)
add_dependencies(entity opengl timer)