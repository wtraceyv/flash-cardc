
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

# --- Fetch FTXUI --------------------------------------------------------------
# https://github.com/ArthurSonzogni/ftxui-starter/blob/master/CMakeLists.txt << the example CMakeLists.txt I got the below from
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v4.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# --------------------------------------------------------------------------------

# specific to my project
project(flash-cardc)

# engine simulator style
add_executable(flash-cardc 
  src/main.cpp

  include/pages.h
  include/gui_custom.h
  include/db.h
)

target_include_directories(flash-cardc PRIVATE include)

# sqlite3 needs manually linked as well (libs installed locally)
target_link_libraries(flash-cardc LINK_PUBLIC sqlite3)

# Obviously these need linked (supplied from ftxui docs/examples)
target_link_libraries(flash-cardc
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)