FLEX_TARGET(lexer
            lexer.flex
            ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)


add_executable(generator
    lexer.yy.cc
    main.cpp
    table_generator.cpp
)

target_link_libraries(generator analyzer)


target_compile_options(generator PRIVATE -fsanitize=address)
target_link_options(generator PRIVATE -fsanitize=address)

target_include_directories(generator
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
