file(GLOB_RECURSE CPPS  ./*.cpp ./*.h)

add_executable(chart ${CPPS} qml.qrc)

target_link_libraries(chart QmlBoard  Qt5::Core Qt5::Quick)

add_custom_command(TARGET chart POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/QmlBoard.dll"
        ${CMAKE_BINARY_DIR}/examples/chart/QmlBoard.dll)
