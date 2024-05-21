FLEX_TARGET(lexer
            lexer.flex
            ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)


add_executable(calculator
    lexer.yy.cc
    main.cpp
    calculator.cpp
)

target_link_libraries(calculator analyzer)


target_compile_options(calculator PRIVATE -fsanitize=address)
target_link_options(calculator PRIVATE -fsanitize=address)

target_include_directories(calculator
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
