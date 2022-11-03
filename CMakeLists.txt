cmake_minimum_required(VERSION 3.5)

project(QmlBoard LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick  REQUIRED)

file(GLOB_RECURSE CPPS  ./*.cpp ./*.h)
LIST(FILTER CPPS EXCLUDE REGEX "examples/*")
add_library(${PROJECT_NAME} SHARED ${CPPS})
target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Core Qt5::Quick
    )
target_compile_definitions(${PROJECT_NAME} PRIVATE HBOARD_LIBRARY)

FILE(GLOB_RECURSE sub_data_dirs RELATIVE "${CMAKE_SOURCE_DIR}" *.h)
LIST(FILTER sub_data_dirs EXCLUDE REGEX ".git.*")
LIST(FILTER sub_data_dirs EXCLUDE REGEX "Install/*")
foreach(data_dir ${sub_data_dirs})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/${data_dir}"
        ${CMAKE_BINARY_DIR}/include/${data_dir}
        COMMENT "Copying  ${data_dir} =>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/${data_dir}.")
endforeach()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dll"
    ${CMAKE_BINARY_DIR}/examples/picture/${PROJECT_NAME}.dll
    COMMENT "Copying  ${data_dir} =>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/${data_dir}.")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dll"
    ${CMAKE_BINARY_DIR}/examples/nodes/${PROJECT_NAME}.dll
    COMMENT "Copying  ${data_dir} =>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/${data_dir}.")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dll"
    ${CMAKE_BINARY_DIR}/examples/handles/${PROJECT_NAME}.dll
    COMMENT "Copying  ${data_dir} =>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/${data_dir}.")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dll"
    ${CMAKE_BINARY_DIR}/examples/chart/${PROJECT_NAME}.dll
    COMMENT "Copying  ${data_dir} =>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/${data_dir}.")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dll"
    ${CMAKE_BINARY_DIR}/examples/trains/${PROJECT_NAME}.dll
    COMMENT "Copying  ${data_dir} =>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/${data_dir}.")

add_subdirectory(examples/picture)
add_subdirectory(examples/nodes)
add_subdirectory(examples/handles)
add_subdirectory(examples/chart)
add_subdirectory(examples/trains)
add_subdirectory(examples/face)
