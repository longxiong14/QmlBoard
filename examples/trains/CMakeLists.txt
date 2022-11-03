file(GLOB_RECURSE CPPS  ./*.cpp ./*.h)

add_executable(trains ${CPPS} qml.qrc)

target_link_libraries(trains QmlBoard  Qt5::Core Qt5::Quick)

add_custom_command(TARGET trains POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/QmlBoard.dll"
        ${CMAKE_BINARY_DIR}/examples/trains/QmlBoard.dll)
