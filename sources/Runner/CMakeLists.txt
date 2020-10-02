cmake_minimum_required(VERSION 3.6) 

set(THIS Runner)

file(GLOB sources_list "${CMAKE_CURRENT_LIST_DIR}/*.cpp")

add_executable(${THIS})

target_sources(${THIS} PRIVATE 
                       ${sources_list}
)

find_package(GTest REQUIRED)

target_link_libraries(${THIS} PRIVATE
                              Adapter
                              GTest::GTest
)

target_compile_features(${THIS} PUBLIC cxx_std_17)

add_custom_command(TARGET ${THIS} POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${PROJECT_SOURCE_DIR}/model
                        $<TARGET_FILE_DIR:${THIS}>/model 
                    COMMENT "Copying LwM2M Model descriptors."
                        VERBATIM
)

add_custom_command(TARGET ${THIS} POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy
                        "${PROJECT_SOURCE_DIR}/loggerConfig.json"
                        $<TARGET_FILE_DIR:${THIS}> 
                    COMMENT "Copying Logger Configuration."
                        VERBATIM
)

add_custom_command(TARGET ${THIS} POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy
                        "${PROJECT_SOURCE_DIR}/serverConfig.json"
                        $<TARGET_FILE_DIR:${THIS}> 
                    COMMENT "Copying Server Configuration."
                        VERBATIM
)
