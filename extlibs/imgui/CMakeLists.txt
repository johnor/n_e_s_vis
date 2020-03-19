include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG 688cf868ea83db6c2958dd6bf7a20d471d00940b # v1.73
)

FetchContent_MakeAvailable(imgui)
