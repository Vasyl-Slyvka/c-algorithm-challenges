if(NOT DEFINED PROGRAM OR NOT DEFINED INPUT_FILE OR NOT DEFINED EXPECTED_FILE)
    message(FATAL_ERROR "CLI test parameters are incomplete")
endif()

execute_process(
    COMMAND "${PROGRAM}"
    INPUT_FILE "${INPUT_FILE}"
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error_output
    RESULT_VARIABLE result
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "CLI exited with ${result}: ${error_output}")
endif()

file(READ "${EXPECTED_FILE}" expected)
string(REPLACE "\r\n" "\n" output "${output}")
string(REPLACE "\r\n" "\n" expected "${expected}")
if(NOT output STREQUAL expected)
    message(FATAL_ERROR "CLI output mismatch.\nExpected:\n${expected}\nActual:\n${output}")
endif()
