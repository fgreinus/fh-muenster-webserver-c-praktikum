/**
 * @file header_fields.h
 * @brief enum for header fields.
 * @author PSE Gruppe 10
 * @date SoSe 2017
 */
# pragma once

/**
 * @brief All types of http headers we support(HEADER_UNKNOWN is used to identify headers we don't)
 */
enum header_field {
    HEADER_HOST,
    HEADER_USER_AGENT,
    HEADER_ACCEPT,
    HEADER_CONNECTION,
    HEADER_CONTENT_LENGTH,
    HEADER_AUTHORIZATION,
    HEADER_UNKNOWN
};
