cmake_minimum_required (VERSION 3.13)

project(board VERSION 1.0.0 LANGUAGES CXX C)

target_sources(${TARGET_NAME} PRIVATE
        board.c
        clock_config.c
        peripherals.c
        pin_mux.c
        )

target_include_directories(${TARGET_NAME} PUBLIC
        .
        )