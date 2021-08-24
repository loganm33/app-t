include_directories(${PROJECT_SOURCE_DIR}/include)
if (${BLD_TYPE} EQUALS W)
    include_directories(${PROJECT_SOURCE_DIR}/build-win/src/app_lib_autogen/include)
elseif (${BLD_TYPE} C)
    include_directories(${PROJECT_SOURCE_DIR}/build-rhel/src/app_lib_autogen/include)
else ()
    include_directories(${PROJECT_SOURCE_DIR}/build-deb/src/app_lib_autogen/include)
endif ()

link_directories(${PROJECT_SOURCE_DIR}/src)

#qt_add_executable(app MANUAL_FINALIZATION app.cpp)
#
#target_link_libraries(app PRIVATE
#        app_lib
#        Qt6::Core
#        Qt6::Gui
#        Qt6::Widgets
#        Qt6::PrintSupport
#)
#
#qt_finalize_executable(app)

add_executable(app app.cpp)

target_link_libraries(app PRIVATE
        app_lib
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::PrintSupport
)

if (UNIX)
    target_compile_options(app PRIVATE -fPIC)
endif ()
