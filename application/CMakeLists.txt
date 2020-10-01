project(n_e_s_vis_app)

include(FetchContent)

# n_e_s
FetchContent_Declare(
    nes_core
    GIT_REPOSITORY https://github.com/evilcorpltd/n_e_s.git
    GIT_TAG 76b16ea70752220e9e71d0e69836c1473c06f51b
)
FetchContent_MakeAvailable(nes_core)

add_executable(${PROJECT_NAME}
   src/control.h
   src/cpu_widget.cpp
   src/cpu_widget.h
   src/gui.h
   src/log_widget.cpp
   src/log_widget.h
   src/main.cpp
   src/ppu_helper.cpp
   src/ppu_helper.h
   src/ppu_widget.cpp
   src/ppu_widget.h
   src/screen.cpp
   src/screen.h
   src/simple_renderer.cpp
   src/simple_renderer.h
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        n_e_s_core
        n_e_s_nes
        n_e_s_disassembler
        sfml-window
        sfml-system
        sfml-graphics
        imgui-sfml
        fmt
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_20
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
