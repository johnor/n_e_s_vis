project(n_e_s_vis_app)

include(FetchContent)

# n_e_s
FetchContent_Declare(
    nes_core
    GIT_REPOSITORY https://github.com/evilcorpltd/n_e_s.git
    GIT_TAG 08b7877c199e4d9b197f233245a29f3d83e1e99a
)

FetchContent_GetProperties(nes_core)
if(NOT nes_core_POPULATED)
    FetchContent_Populate(nes_core)
    add_subdirectory(${nes_core_SOURCE_DIR} ${nes_core_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(${PROJECT_NAME}
   src/main.cpp
   src/cpu_widget.h
   src/cpu_widget.cpp
   src/ppu_widget.h
   src/ppu_widget.cpp
   src/screen.h
   src/screen.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        n_e_s_core
        n_e_s_nes
        sfml-window
        sfml-system
        sfml-graphics
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
