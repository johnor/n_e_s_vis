include(FetchContent)

FetchContent_Declare(imgui-sfml
    GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
    GIT_TAG 48d527d0ca444aa546108103cc0ad07b0949bdd6
)

FetchContent_GetProperties(imgui
    SOURCE_DIR imgui_SOURCE_DIR
)

set(IMGUI_SFML_FIND_SFML FALSE)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

FetchContent_MakeAvailable(imgui-sfml)

add_library(imgui-sfml ALIAS ImGui-SFML)
