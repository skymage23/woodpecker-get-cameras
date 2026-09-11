if(WIN32) 
    add_library(get_cameras_internal
        "${CMAKE_CURRENT_LIST_DIR}/Windows/get_cameras_windows.cc"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/err_handling.cc"
        "${CMAKE_CURRENT_LIST_DIR}/Windows/exceptions.cc"
        "${CMAKE_CURRENT_LIST_DIR}/exceptions.cc"
        "${CMAKE_CURRENT_LIST_DIR}/err_handling.cc"
    )

    target_link_libraries(get_cameras_internal
        PRIVATE setupapi
        PRIVATE cfgmgr32
    )

    target_include_directories(get_cameras_internal
        PRIVATE "${ctre}"
    )
elseif(LINUX)
    add_library(get_cameras_internal
        "${CMAKE_CURRENT_LIST_DIR}/Linux/get_cameras_linux.cc"
        "${CMAKE_CURRENT_LIST_DIR}/Linux/err_handling.cc"
        "${CMAKE_CURRENT_LIST_DIR}/err_handling.cc"
    )

else()
    message(FATAL_ERROR "Not a supported OS.")
endif()

target_include_directories(get_cameras_internal
    PUBLIC "${CMAKE_SOURCE_DIR}/include"
    PUBLIC "${mlohmann_json}"
)

add_executable(get_cameras
    get_cameras.cc
)

add_dependencies(get_cameras 
    get_cameras_internal
)

target_include_directories(get_cameras
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE "${mlohmann_json}"
)