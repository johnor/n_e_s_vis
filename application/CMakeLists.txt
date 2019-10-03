project(n_e_s_vis_app)

include(FetchContent)

# n_e_s
FetchContent_Declare(
    nes_core
    GIT_REPOSITORY https://github.com/evilcorpltd/n_e_s.git
    GIT_TAG 23b149dbd3deaf3307efce693db5ea79e09b6818
)

FetchContent_GetProperties(nes_core)
if(NOT nes_core_POPULATED)
    FetchContent_Populate(nes_core)
    add_subdirectory(${nes_core_SOURCE_DIR} ${nes_core_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(${PROJECT_NAME}
   src/main.cpp
   src/nes.cpp
   src/nes.h
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
