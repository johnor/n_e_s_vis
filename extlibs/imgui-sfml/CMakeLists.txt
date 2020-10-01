include(FetchContent)

FetchContent_Declare(imgui-sfml
    GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
    GIT_TAG        5ab660a13bc997225587eea88755f22f999f7ba9
)

FetchContent_GetProperties(imgui
    SOURCE_DIR imgui_SOURCE_DIR
)

set(IMGUI_SFML_FIND_SFML FALSE)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

FetchContent_MakeAvailable(imgui-sfml)

add_library(imgui-sfml ALIAS ImGui-SFML)
