add_an_library(
        Ang SHARED
        Lexer.cpp
        TokenKinds.cpp
        AST.cpp
        Parser.cpp
)

target_link_libraries(Ang PUBLIC Basic)