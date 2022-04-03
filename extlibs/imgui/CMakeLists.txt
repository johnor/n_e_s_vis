include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG 377b864bb4a9ed9a25674b04f64f63baf221060d # latest docking
)

FetchContent_MakeAvailable(imgui)
