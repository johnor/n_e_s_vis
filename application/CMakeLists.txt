project(n_e_s_vis_app)

include(FetchContent)

# n_e_s
FetchContent_Declare(
    nes_core
    GIT_REPOSITORY https://github.com/evilcorpltd/n_e_s.git
    GIT_TAG e82eb535d825c2d241d31ba980102e14821f014b
)
FetchContent_MakeAvailable(nes_core)

add_executable(${PROJECT_NAME}
   src/cpu_widget.cpp
   src/cpu_widget.h
   src/log_widget.cpp
   src/log_widget.h
   src/main.cpp
   src/ppu_widget.cpp
   src/ppu_widget.h
   src/screen.cpp
   src/screen.h
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
