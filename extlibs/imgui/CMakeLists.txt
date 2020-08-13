include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG a9626e1162c3742307553108aff8a443c20bb73c # latest docking
)

FetchContent_MakeAvailable(imgui)
