add_library(core OBJECT
    basic_game.hpp
    game.hpp
    game.cpp
    map_editor.hpp
    map_editor.cpp
)
target_compile_features(core PRIVATE cxx_std_20)
target_include_directories(core
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PRIVATE ${PROJECT_BINARY_DIR}/src
)
add_dependencies(core opengl timer)