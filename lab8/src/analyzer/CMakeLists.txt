add_library(analyzer
    parser.cpp
    node.cpp
    token.cpp
    position.cpp
    scanner.cpp
)

target_include_directories(analyzer
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
