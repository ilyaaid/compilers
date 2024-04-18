FLEX_TARGET(lexer
            lexer.flex
            ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)


message("=======",${FLEX_lexer_OUTPUTS})
add_executable(exec_lab9
    lexer.yy.cc
    main.cpp
    position.cpp
    scanner.cpp
    parser.cpp
    node.cpp
    token.cpp
    rule.cpp
)


target_compile_options(exec_lab9 PRIVATE -fsanitize=address)
target_link_options(exec_lab9 PRIVATE -fsanitize=address)

target_include_directories(exec_lab9 PUBLIC include)
