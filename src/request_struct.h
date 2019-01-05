/**
 * @file request_struct.h
 * @brief The struct for all request information
 * @author PSE Gruppe 10
 * @date SoSe 2017
 */

#pragma once

/**
 * @brief A struct to hold all information we parse out of the request to further process it within the application
 */
typedef struct {
    char *method;           /**< The HTTP Method */
    char *uri;              /**< The requested URI */
    char *protocol;         /**< The used protocol */
    char *host;             /**< The requested host */
    char *user_agent;       /**< The user agent information on the client */
    char *accept;           /**< The accepted response protocols */
    char *connection;       /**< Information on the connection state */
    char *content_length;   /**< The length of the content provided */
    char *authorization;    /**< The Authorization Header */
} request_header;
