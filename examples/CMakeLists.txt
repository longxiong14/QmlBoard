file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(example2 ${CPPS} qml.qrc)

target_link_libraries(example2 QmlBoard ${OpenCV_LIBS} Qt5::Core Qt5::Quick)

