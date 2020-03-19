include(FetchContent)

FetchContent_Declare(imgui-sfml
    GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
    GIT_TAG        f5aec785a9f6e942d94551313fa359d6d0d094a9
)

FetchContent_GetProperties(imgui
    SOURCE_DIR imgui_SOURCE_DIR
)

set(IMGUI_SFML_FIND_SFML FALSE)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

FetchContent_MakeAvailable(imgui-sfml)

add_library(imgui-sfml ALIAS ImGui-SFML)
