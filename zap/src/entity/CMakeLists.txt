add_library(entity OBJECT
    weapon.cpp
    entity.cpp
    inventory.cpp
    projectile.cpp
    player3d.cpp
)
target_compile_features(entity PRIVATE cxx_std_20)
target_include_directories(entity
    PRIVATE ${PROJECT_SOURCE_DIR}/src
    PRIVATE ${PROJECT_BINARY_DIR}/src
)
add_dependencies(entity opengl timer)