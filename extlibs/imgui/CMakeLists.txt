include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG 958e58b06b35ca845a2e8f5bd6032debd3cec0c0 # latest docking
)

FetchContent_MakeAvailable(imgui)
