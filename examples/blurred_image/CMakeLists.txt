file(GLOB_RECURSE CPPS  ./*.cpp ./*.h)

add_executable(blurred_image ${CPPS} qml.qrc)

target_link_libraries(blurred_image QmlBoard  Qt5::Core Qt5::Quick)

add_custom_command(TARGET blurred_image POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/QmlBoard.dll"
        ${CMAKE_BINARY_DIR}/examples/blurred_image/QmlBoard.dll)
