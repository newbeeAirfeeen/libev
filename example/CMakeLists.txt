aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} test_src)

foreach(item ${test_src})
    string(REGEX MATCH ".*[/\\](.*).c$" item_src ${item})
    message("  + ${CMAKE_MATCH_1}")
    add_executable(${CMAKE_MATCH_1} ${item})
    target_link_libraries(${CMAKE_MATCH_1} ev)
endforeach()