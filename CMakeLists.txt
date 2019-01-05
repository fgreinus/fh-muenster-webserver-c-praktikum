cmake_minimum_required(VERSION 3.7)
project(Gruppe10)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES src/echo_server.c src/response_status_code.h src/request.c src/request.h src/utils.c src/utils.h src/echo_server.h src/tables.c src/tables.h src/response.c src/poison.h src/response.h src/mime.c src/mime.h src/response_struct.h src/request_struct.h src/header.c src/header.h src/mime_types.h src/header_fields.h src/base64.c src/base64.h src/auth.c src/auth.h)
add_executable(Gruppe10 ${SOURCE_FILES})

target_link_libraries(Gruppe10 ssl crypto)