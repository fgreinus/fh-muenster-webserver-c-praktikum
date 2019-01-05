/**
 * @file echo_server.h
 * @author PSE Gruppe 10
 * @date SoSe 2017
 * @brief Basic definitions used in the whole codebase
 */

#pragma once

#include "poison.h"
#include "request.h"
#include "response.h"
#include "utils.h"

/** @brief The Resource URI to reach the debug site */
#define DEBUG_URI "/debug"

/** @brief The document root (all requests made will only access content in here - except for error pages) */
#define DOCUMENT_ROOT_BASE_PATH "../vhosts/"

/** @brief The name it the intern vhost */
#define VHOST_INTERN_NAME "intern"

/** @brief The name of the extern vhost */
#define VHOST_EXTERN_NAME "extern"

/** @brief The path to the intern vhost (document root and vhost name concatenated) */
#define VHOST_INTERN_PATH DOCUMENT_ROOT_BASE_PATH VHOST_INTERN_NAME

/** @brief The path to the extern vhost (document root and vhost name concatenated) */
#define VHOST_EXTERN_PATH DOCUMENT_ROOT_BASE_PATH VHOST_EXTERN_NAME

/** @brief The path to the htpasswd file (including the document root) */
#define HTPASSWD_PATH DOCUMENT_ROOT_BASE_PATH "htpasswd"

/** @brief The string holding the absolute path to the document root */
char *docroot;

/** @brief The string holding the absolute path of the intern vhost directory */
char *intern_path;

/** @brief The string holding the absolute path of the extern vhost directory */
char *extern_path;