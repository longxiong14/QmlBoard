file(GLOB_RECURSE CPPS  ./*.cpp ./*.h)

add_executable(example ${CPPS} qml.qrc)

target_link_libraries(example QmlBoard ${OpenCV_LIBS} Qt5::Core Qt5::Quick)

