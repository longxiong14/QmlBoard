FILE(GLOB_RECURSE CPPS  ./*.cpp ./*.h)
LIST(FILTER CPPS EXCLUDE REGEX "install/*")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(opencv ${CPPS} qml.qrc)
target_link_libraries(opencv QmlBoard  Qt5::Core Qt5::Quick
    ${OpenCV_LIBS})

add_custom_command(TARGET opencv POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/QmlBoard.dll"
        ${CMAKE_BINARY_DIR}/examples/opencv/QmlBoard.dll)
