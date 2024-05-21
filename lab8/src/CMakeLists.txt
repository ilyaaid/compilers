FLEX_TARGET(lexer
            lexer.flex
            ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)


message("=======",${FLEX_lexer_OUTPUTS})
add_executable(exec_lab8
    lexer.yy.cc
    table_generator_main.cpp
    table_generator.cpp
    position.cpp
    scanner.cpp
    parser.cpp
    node.cpp
    token.cpp
)


target_compile_options(exec_lab8 PRIVATE -fsanitize=address)
target_link_options(exec_lab8 PRIVATE -fsanitize=address)

target_include_directories(exec_lab8 PUBLIC include)
