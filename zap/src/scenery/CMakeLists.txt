add_library(scenery OBJECT
    camera3d.cpp
    map_util.cpp
    hud.cpp
)
target_compile_features(scenery PRIVATE cxx_std_20)
target_include_directories(scenery
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PRIVATE ${PROJECT_BINARY_DIR}/src
)
add_dependencies(scenery opengl timer)