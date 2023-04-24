add_an_library(
        liblr SHARED
        State.cpp
        Parser.cpp
        Table.cpp
        Analyzer.cpp
)

target_link_libraries(liblr PUBLIC Basic libgrammar)