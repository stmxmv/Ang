add_an_tool(LR main.cpp)
target_link_libraries(LR PRIVATE Basic liblr)

add_an_tool(LRTable table.cpp)
target_link_libraries(LRTable PRIVATE Basic liblr)

add_an_tool(LRAnalyzer analyzer.cpp)
target_link_libraries(LRAnalyzer PRIVATE Basic liblr)