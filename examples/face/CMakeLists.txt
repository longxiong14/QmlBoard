file(GLOB_RECURSE CPPS  ./*.cpp ./*.h)

add_executable(face ${CPPS} qml.qrc)

target_link_libraries(face QmlBoard  Qt5::Core Qt5::Quick)

add_custom_command(TARGET face POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/QmlBoard.dll"
        ${CMAKE_BINARY_DIR}/examples/face/QmlBoard.dll)
