include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG 266dff9bed8d2b4efd103591f25299e38f6d7921 # v1.76 + docking
)

FetchContent_MakeAvailable(imgui)
