project(n_e_s_vis_app)

include(FetchContent)

# n_e_s
FetchContent_Declare(
    nes_core
    GIT_REPOSITORY https://github.com/evilcorpltd/n_e_s.git
)

FetchContent_GetProperties(nes_core)
if(NOT googletest_POPULATED)
    FetchContent_Populate(nes_core)
    add_subdirectory(${nes_core_SOURCE_DIR} ${nes_core_BINARY_DIR})
endif()

add_executable(${PROJECT_NAME}
   src/main.cpp
   src/nes.cpp
   src/nes.h
   src/nes_widget.h
   src/nes_widget.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        n_e_s_core
        sfml-window
        sfml-system
        sfml-graphics
        sfml-main
        imgui-sfml
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_17
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
