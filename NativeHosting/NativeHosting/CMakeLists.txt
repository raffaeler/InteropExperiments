# CMakeList.txt : CMake project for NativeHosting, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# https://cmake.org/cmake/help/latest/prop_tgt/CXX_STANDARD.html
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source to this project's executable.
# https://stackoverflow.com/questions/36174499/why-add-header-files-into-add-library-add-executable-command-in-cmake
add_executable (NativeHosting "NativeHosting.cpp" "NativeHosting.h")

# set NativeHosting as the default startup project in cmake-generated solutions
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT NativeHosting)

# TODO: Add tests and install targets if needed.


# === cmake ignore the following ===
# add_compile_definitions(WIN64)
#add_link_options(NativeHosting "-ldl")
#add_compile_options(NativeHosting "-ldl")
#set(CMAKE_EXE_LINKER_FLAGS, "-ldl")
#set(CMAKE_SHARED_LINKER_FLAGS, "-ldl")

IF(WIN32)

add_custom_command(TARGET NativeHosting POST_BUILD                      # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different                       # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/config-win-x64.json"     # <--this is in-file
        $<TARGET_FILE_DIR:NativeHosting>/config.json)                   # <--this is out-file path

ELSE(WIN32)

target_link_options(NativeHosting PUBLIC -ldl)
add_custom_command(TARGET NativeHosting POST_BUILD                      # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different                       # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/config-linux-x64.json"   # <--this is in-file
        $<TARGET_FILE_DIR:NativeHosting>/config.json)                   # <--this is out-file path

ENDIF(WIN32)




add_custom_command(TARGET NativeHosting POST_BUILD              # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different               # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/cd.xml"          # <--this is in-file
        $<TARGET_FILE_DIR:NativeHosting>)                       # <--this is out-file path


