add_an_library(
        libgrammar SHARED
        Lexer.cpp
        Parser.cpp
        Grammar.cpp
        AST.cpp
)

target_link_libraries(libgrammar PUBLIC Basic)