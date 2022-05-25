add_library(timer OBJECT
    timer.cpp
)
target_include_directories(timer
    PRIVATE ${PROJECT_BINARY_DIR}/src
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)
target_compile_features(timer PRIVATE cxx_std_20)