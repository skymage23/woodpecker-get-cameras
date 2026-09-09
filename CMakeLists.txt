cmake_minimum_required(VERSION 4.0.0)
# Refuse compilation on all platforms except Windows
# Re: this project uses foundational components of Windows.
if(NOT WIN32)
    message(FATAL_ERROR "This project can only be compiled on Windows operating systems.")
endif()

set(submodules_dir "${CMAKE_CURRENT_LIST_DIR}/submodules")
set(cmake_build_sys_utilities "${submodules_dir}/cmake-build-sys-utilities")
if(NOT IS_DIRECTORY "${cmake_build_sys_utilities}")
    message(FATAL_ERROR "\"cmake_build_sys_utilities\" submodule not detected on the filesystem.  Did you initialize and update the submodules?")
endif()


include("${submodules_dir}/cmake-build-sys-utilities/scripts/debug.cmake")

set( mlohmann_json "${submodules_dir}/mlohmann_json/include")
assert_is_directory("${mlohmann_json}")

set(ctre "${CMAKE_CURRENT_LIST_DIR}/submodules/ctre/include")
assert_is_directory("${ctre}")

project(win_get_cameras LANGUAGES CXX)

# Specify the C++ standard (e.g., 11, 14, 17, 20, 23, 26)
set(CMAKE_CXX_STANDARD 20)

# Enforce the standard (fails to compile if the compiler doesn't support it)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Disable compiler-specific extensions (forces standard compliance, e.g., -std=c++17 instead of -std=gnu++17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)