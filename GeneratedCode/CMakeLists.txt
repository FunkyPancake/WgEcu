cmake_minimum_required(VERSION 3.13)


project(GeneratedCode LANGUAGES CXX C)

target_sources(
    ${TARGET_NAME}
    PRIVATE
    board/board.c
    board/clock_config.c
    board/peripherals.c
    board/pin_mux.c

)

target_include_directories(
    ${TARGET_NAME}
    PUBLIC
    board
)