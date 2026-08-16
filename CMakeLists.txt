cmake_minimum_required(VERSION 3.20)

project(
    c_algorithm_challenges
    VERSION 1.0.0
    DESCRIPTION "Tested algorithmic problem solving in C11"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(AC_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
option(AC_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

function(ac_configure_target target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
        if(AC_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wshadow
        )
        if(AC_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
        if(AC_ENABLE_SANITIZERS)
            target_compile_options(${target} PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
            )
            target_link_options(${target} PRIVATE -fsanitize=address,undefined)
        endif()
    endif()
endfunction()

add_library(algorithm_challenges
    src/bst_shapes.c
    src/cheese.c
    src/digital_root.c
    src/speed_adjustment.c
)
target_include_directories(algorithm_challenges
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
ac_configure_target(algorithm_challenges)

function(ac_add_solver target source output_name)
    add_executable(${target} ${source})
    set_target_properties(${target} PROPERTIES OUTPUT_NAME ${output_name})
    target_link_libraries(${target} PRIVATE algorithm_challenges)
    if(NOT MSVC)
        target_link_libraries(${target} PRIVATE m)
    endif()
    ac_configure_target(${target})
endfunction()

ac_add_solver(cutting_cheese apps/cutting_cheese.c cutting-cheese)
ac_add_solver(bst_shapes apps/bst_shapes.c bst-shapes)
ac_add_solver(digital_root apps/digital_root.c digital-root)
ac_add_solver(speed_adjustment apps/speed_adjustment.c speed-adjustment)

include(CTest)

if(BUILD_TESTING)
    foreach(test_name IN ITEMS cheese bst_shapes digital_root speed_adjustment)
        add_executable(test_${test_name} tests/test_${test_name}.c)
        target_link_libraries(test_${test_name} PRIVATE algorithm_challenges)
        if(NOT MSVC)
            target_link_libraries(test_${test_name} PRIVATE m)
        endif()
        ac_configure_target(test_${test_name})
        add_test(NAME ${test_name}_unit COMMAND test_${test_name})
    endforeach()

    add_test(NAME cutting_cheese_help COMMAND cutting_cheese --help)
    add_test(NAME bst_shapes_help COMMAND bst_shapes --help)
    add_test(NAME digital_root_help COMMAND digital_root --help)
    add_test(NAME speed_adjustment_help COMMAND speed_adjustment --help)

    function(ac_add_cli_test test_name target fixture)
        add_test(
            NAME ${test_name}
            COMMAND ${CMAKE_COMMAND}
                -DPROGRAM=$<TARGET_FILE:${target}>
                -DINPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/${fixture}.in
                -DEXPECTED_FILE=${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/${fixture}.out
                -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/run_cli_test.cmake
        )
    endfunction()

    ac_add_cli_test(cutting_cheese_cli cutting_cheese cheese)
    ac_add_cli_test(bst_shapes_cli bst_shapes bst)
    ac_add_cli_test(digital_root_cli digital_root digital_root)
    ac_add_cli_test(speed_adjustment_cli speed_adjustment speed)
endif()

install(
    TARGETS cutting_cheese bst_shapes digital_root speed_adjustment
    RUNTIME DESTINATION bin
)
