cmake_minimum_required(VERSION 3.8)
project(PackageTest CXX)

set(VERBOSE_FILE_INCLUSION ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(example example.cpp)

find_package(GTest REQUIRED)

target_link_libraries(example PUBLIC 
                              ${CONAN_LIBS}
                              GTest::GTest
)

set_target_properties(example PROPERTIES
                              CXX_STANDARD 17
                              CXX_STANDARD_REQUIRED ON
)

add_custom_command(TARGET example POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${PROJECT_SOURCE_DIR}/model
                        $<TARGET_FILE_DIR:example>/model 
                    COMMENT "Copying LwM2M Model descriptors."
                        VERBATIM
)

add_custom_command(TARGET example POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy
                        "${PROJECT_SOURCE_DIR}/loggerConfig.json"
                        $<TARGET_FILE_DIR:example> 
                    COMMENT "Copying Logger Configuration."
                        VERBATIM
)

add_custom_command(TARGET example POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy
                        "${PROJECT_SOURCE_DIR}/serverConfig.json"
                        $<TARGET_FILE_DIR:example> 
                    COMMENT "Copying Server Configuration."
                        VERBATIM
)