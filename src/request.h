/**
 * @file request.h
 * @author PSE Gruppe 10
 * @date SoSe 2017
 * @brief All functions needed for parsing and processing an incoming http request
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "auth.h"
#include "header.h"
#include "mime.h"
#include "request_struct.h"
#include "response_struct.h"
#include "tables.h"
#include "utils.h"

/**
 * @brief default buffer size
 *
 * This is set to 8192, because this also is the default value in the well known apache2 webserver
 */
#define BUFFER_SIZE 8192

/**
 * @brief the default size for a http header (we decided to use this specific value, as it turned out to fit very well)
 */
#define HEADER_VALUE_SIZE 64

/**
 * Parses an incoming request and does a many sanity checks (see RCF HTTP 1.0/1.1)
 * The method itself directly throws http errors in case something does not fulfill standards or is badly formed
 * Headed which are valid and not supported are ignored
 * All detected Headers are stored in a struct
 *
 * Further information on all the checks can be found before each snippet in this method
 *
 * @param buffer_in the input buffer taken from the socket
 * @param req_header The (publicly available) object that will hold all request-related information
 * @param response The (publicly available) object that's reserved to hold the outgoing response
 * @see https://tools.ietf.org/html/rfc1945
 * @see https://tools.ietf.org/html/rfc7230
 */
void parse_request(char *, request_header *, http_response *);

/**
 *
 * @brief Allocates all memory needed to store information regarding the incoming request
 *
 * @param response response header which is allocated
 * @return true if the allocation was successful, false otherwise
 */
bool allocate_header(request_header *);

/**
 * @brief Frees memory we allocated using allocate_header
 *
 * @param header a pointer to the header we are about to free
 * @see allocate_header
 */
void free_header(request_header *);

/**
 *  @brief Checks if the request is valid (method, host, content-length, authorization, and some more)
 *
 *  - checks if there's an host provided
 *  - checks if the http method is GET (we currently support no other method)
 *  - if the requested host is intern, checks if there's the required authorization header (otherwise requests authorization)
 *  - checks if the content length header's value is a valid integer
 *  - checks if there is a %-char as it is not allowed after decoding the url
 *
 *  Calls <tt>build_absolute_path</tt> when everything went well
 *
 *  @param header the header we just filled with information during parse_request
 *  @param response a pointer to the struct we want to store the response information in
 *  @see parse_request
 *  @see build_absolute_path
 */
void validate_request(request_header *, http_response *);

/**
 *
 *
 * @brief Takes the parsed requested resource and generates an internally processable resource path (if valid)
 *
 * Takes the parsed requested resource and checks if:
 * - it is within the directory root (making use of realpath i.e.)
 * - it is within the RFC boundaries (URI length)
 * - it is a directory and the request actually wants to see the index.html file
 * - it's within it's area (intern should only access internal content, extern only external)
 *
 * Responds with an http error response, otherwise sets the correct resource_path in the response struct
 *
 * @param request_reader the request struct with all information gathered from the request
 * @param response the response struct used for either errors or the resulting resource path
 * @param vhost the accessed vhost (it is appended to the resource path
 *
 * @see http_response
 */
void build_absolute_path(request_header *, http_response *, const char *);