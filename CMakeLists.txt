cmake_minimum_required(VERSION 3.21)

project(RoozegaarCalendar LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_WIN32_EXECUTABLE TRUE)

include_directories(include)
include_directories(resources)

add_executable(RoozegaarCalendar
    resources/app.rc 
    src/main.c
    src/window.c
    src/tray.c
    src/calendar.c
    src/persian_number.c
    src/startup.c "include/ui_font.h" "src/ui_font.c")

target_compile_definitions(RoozegaarCalendar PRIVATE UNICODE _UNICODE)

target_link_libraries(RoozegaarCalendar
    user32
    gdi32
    shell32
    advapi32
)
