cmake_minimum_required(VERSION 3.22)
project(edlisp C)

find_package(FLEX 2.6 REQUIRED)
find_package(BISON 3.0 REQUIRED)

set(CMAKE_C_STANDARD 23)
set(SRC_DIR "src")
set(OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(LEXER_OUT "${OUT_DIR}/lexer.c")
set(PARSER_OUT "${OUT_DIR}/parser.c")

flex_target(LEXER "${SRC_DIR}/lexer.l" "${LEXER_OUT}" DEFINES_FILE "${OUT_DIR}/lexer.h")
bison_target(PARSER "${SRC_DIR}/parser.y" "${PARSER_OUT}" DEFINES_FILE "${OUT_DIR}/parser.h")
add_flex_bison_dependency(LEXER PARSER)

add_executable(edlisp "${SRC_DIR}/main.c" "${SRC_DIR}/lisp.h" "${SRC_DIR}/lisp.c" "${LEXER_OUT}" "${PARSER_OUT}"
        src/edlisp_eval.h
        src/edlisp_eval.c
        src/cli_app.h
        src/cli_app.c
        src/file_app.h
        src/file_app.c
        )
target_include_directories(edlisp PRIVATE "${OUT_DIR}")
target_include_directories(edlisp PRIVATE "${SRC_DIR}")
