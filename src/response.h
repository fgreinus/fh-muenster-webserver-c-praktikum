/**
 * @file response.h
 * @brief Methods needed to compute a response to the incoming http request
 * @author PSE Gruppe 10
 * @date SoSe 2017
 */

#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request_struct.h"
#include "response_struct.h"
#include "utils.h"

/** @brief The initial size of each buffer used to store output data (big enough for simple responses, will otherwise be extended by <tt>response_buffer_attach_dynamic</tt>) */
#define BUFSIZE_INITIAL 10240

/** @brief The base path to the error pages directory */
#define ERROR_PAGE_BASE_PATH "../error_pages/"

/**
 * @brief Takes a response buffer and doubles it's size (if possible) and then write in the @p new_part
 * @param buffer buffer where the new_part shall be attached
 * @param new_part new part which is attached
 * @param new_part_len length of the new part
 * @return size_t length of the resized buffer
 */
size_t response_buffer_attach_dynamic(http_response_buffer *, char *, size_t);

/**
 * @brief Resets the content of the response struct to it's initial state (used when building error responses i.e.)
 * @param response which should be resetted
 */
void response_reset(http_response *);

/**
 * @brief loads file @p file_name and attaches the content to the response's body
 *
 * - if file does not exist or is not readable a 404 response is created
 *
 * @param response response where the file content is attached
 * @param file_name name of the file which should be attached
 *
 * @return length of the files content and 0 if the file does not exist or is not readable
 */
size_t response_load_file(http_response *, char *);

/**
 * @brief Creates an empty response (and allocates memory by calling <tt>response_initialize</tt>)
 * @param status_code status code of new response
 * @return pointer to response
 * @warning as we allocate memory here, the consumer will need to free it itself by using <tt>free_response</tt>
 */
http_response *response_create(enum response_status_code);

/**
 *  @brief creates the response headers and attaches them to the response
 *  - HTTP Protocol
 *  - Status Code
 *  - Content Type
 *  - Current Date & Time
 *  - Server Information
 *  - Connection State Information (Close)
 *  - Content-Type
 *  - Content-Length
 *  - Header/Body Separator
 *  @param response response where the headers are attached
 *  @param status_code status code of the response
 */
void response_build_header(http_response *, enum response_status_code);

/**
 * @brief Frees the memory allocated by <tt>response_initialize</tt>
 * @param response Pointer to the http response to be freed
 */
void free_response(http_response *);

/**
 * @brief builds a debug response with request headers in the body
 * @param header header of the request where the error occurred
 * @param response response pointer to fill the debug response in
 */
void build_debug_response(request_header *, http_response *);

/**
 * @brief Builds a response using the information gathered to this point
 * - if debug response shall be built, we simply call <tt>build_debug_response</tt>
 * - we load and attach the file by calling <tt>response_load_file</tt>
 * @param request headers of the request
 * @param response to put the information in
 */
void process_request_to_response(request_header *, http_response *);

/**
 * @brief Creates an error response
 * This function looks for a file with the scheme STATUS_CODE.html in the error-pages directory.
 * If none is found, it loads the general.html file and builds a response using the loaded error file.
 *
 * @param response Pointer to fill the response in
 * @param response_status_code The status code of the error
 */
void build_error_response(http_response *, enum response_status_code);