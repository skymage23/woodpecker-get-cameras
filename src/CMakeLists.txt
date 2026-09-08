add_executable(get_cameras
    get_cameras.cc
)

target_link_libraries(get_cameras 
PRIVATE setupapi
PRIVATE cfgmgr32
)

target_include_directories(get_cameras
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)