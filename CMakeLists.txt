cmake_minimum_required(VERSION 4.0.0)
# Refuse compilation on all platforms except Windows
# Re: this project uses foundational components of Windows.
if(NOT WIN32)
    message(FATAL_ERROR "This project can only be compiled on Windows operating systems.")
endif()

project(win_get_cameras)

add_subdirectory(src)