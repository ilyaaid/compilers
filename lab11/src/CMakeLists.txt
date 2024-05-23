
FLEX_TARGET(lexer
            lexer.flex
            ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)

BISON_TARGET(MyParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser_generated.cpp
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser_generated.hpp)


add_executable(formatter
    main.cpp
    scanner.cpp
    driver.cpp

    lexer.yy.cc
    parser_generated.cpp
)

target_compile_options(formatter PRIVATE -fsanitize=address)
target_link_options(formatter PRIVATE -fsanitize=address)

target_include_directories(formatter
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
)
