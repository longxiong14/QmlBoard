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

message("CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR})

FILE(GLOB_RECURSE sub_data_dirs RELATIVE
    ${CMAKE_SOURCE_DIR}/examples/opencv/icons/
    *.png *.jpg)
foreach(data_dir ${sub_data_dirs})
#    message("copy dir : " ${CMAKE_SOURCE_DIR}/examples/opencv/icons/${data_dir}
#        " to "            ${CMAKE_BINARY_DIR}/examples/opencv/icons/${data_dir})
    add_custom_command(TARGET opencv POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/examples/opencv/icons/${data_dir}
        ${CMAKE_BINARY_DIR}/examples/opencv/icons/${data_dir}
        COMMENT "Copying  ${data_dir} =>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/${data_dir}.")
endforeach()
