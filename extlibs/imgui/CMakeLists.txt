include(FetchContent)

FetchContent_Declare(
  imgui
  URL "https://github.com/ocornut/imgui/archive/v1.73.zip"
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

set(ImGui_ROOT ${imgui_SOURCE_DIR} PARENT_SCOPE)

file(GLOB ImGui_SOURCES "${imgui_SOURCE_DIR}/*.cpp")
set(ImGui_SOURCES ${ImGui_SOURCES} PARENT_SCOPE)

file(GLOB ImGui_HEADERS "${imgui_SOURCE_DIR}/*.h")
set(ImGui_HEADERS ${ImGui_HEADERS} PARENT_SCOPE)
