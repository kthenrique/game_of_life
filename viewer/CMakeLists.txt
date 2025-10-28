include(FetchContent)
 
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9  # Replace with a version, tag, or commit hash
)
 
FetchContent_MakeAvailable(ftxui)
 
add_executable(viewer main.cpp)

target_include_directories(viewer PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/${IMPLEMENTATION}/inc/)

target_link_libraries(viewer
  PRIVATE
  ftxui::screen
  ftxui::dom
  ftxui::component
  GameOfLifeLib
)
