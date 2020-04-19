include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG 5503c0a12e0c929e84b3f61b2cb4bb9177ea3da1 # v1.76
)

FetchContent_MakeAvailable(imgui)
