file(GLOB_RECURSE CPPS  ./*.cpp ./*.h)

add_executable(handles ${CPPS} qml.qrc)

target_link_libraries(handles QmlBoard  Qt5::Core Qt5::Quick)

add_custom_command(TARGET handles POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/QmlBoard.dll"
        ${CMAKE_BINARY_DIR}/examples/handles/QmlBoard.dll)
