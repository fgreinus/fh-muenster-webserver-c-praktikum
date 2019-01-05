/*! \file response_struct.h
 *  \brief struct for responses.
 */

#pragma once

#include "mime_types.h"
#include "response_status_code.h"

/*! \struct messages[]
 *  \brief staus codes end their string value
 */
static const struct {
    enum response_status_code number;
    char *message;
} messages[] = {
        {STATUS_200_OK,                    "OK"},
        {STATUS_201_CREATED,               "Created"},
        {STATUS_202_ACCEPTED,              "Accepted"},
        {STATUS_204_NO_CONTENT,            "No Content"},

        {STATUS_301_MOVED_PERMANENTLY,     "Moved Permanently"},
        {STATUS_302_MOVED_TEMPORARILY,     "Moved Temporarily"},
        {STATUS_304_NOT_MODIFIED,          "Not Modified"},

        {STATUS_400_BAD_REQUEST,           "Bad Request"},
        {STATUS_401_UNAUTHORIZED,          "Unauthorized"},
        {STATUS_403_FORBIDDEN,             "Forbidden"},
        {STATUS_404_NOT_FOUND,             "Not Found"},

        {STATUS_500_INTERNAL_SERVER_ERROR, "Internal Server Error"},
        {STATUS_501_NOT_IMPLEMENTED,       "Not Implemented"},
        {STATUS_502_BAD_GATEWAY,           "Bad Gateway"},
        {STATUS_503_SERVICE_UNAVAILABLE,   "Service Unavailable"}
};

/*! \struct http_response_buffer
 *  \brief response buffer
 *  \var buffer response buffer
 *  \var end length of the contend saved in buffer
 *  \var size max size of buffer
 */
typedef struct {
    char *buffer;
    size_t end;
    size_t size;
} http_response_buffer;

/*! \struct http_response
 *  \brief struct for responses with buffer for header body
 */
typedef struct {
    http_response_buffer *header;
    http_response_buffer *body;
    char *resource_path;
    bool is_debug;
    bool is_intern;
    bool is_error_response;
    bool is_ready;
    enum response_status_code status_code;
    enum mime_type mime_type;
} http_response;