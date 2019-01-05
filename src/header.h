/*! \file header.h
 *  \brief header structs and methods.
 */
# pragma once

#include <string.h>
#include "header_fields.h"

/*! \var static const struct header_field[]
 * \brief header types which are processed.
 *          types are:
 *              -# host
 *              -# user-agent
 *              -# accept
 *              -# connection
 *              -# content-length
 */

static const struct {
    char *name;
    enum header_field identifier;

} header_field[] = {
        {"host",           HEADER_HOST},
        {"user-agent",     HEADER_USER_AGENT},
        {"accept",         HEADER_ACCEPT},
        {"connection",     HEADER_CONNECTION},
        {"content-length", HEADER_CONTENT_LENGTH},
        {"authorization",  HEADER_AUTHORIZATION}
};

/*! \fn enum header_field match_header_field(char *, size_t)
 *
 * \param name name of the header as enum
 * \param length length of the name
 * \return returnes header identifier from header_field[] or HEADER_UNKNOWN if header not listed
 */
enum header_field match_header_field(char *, size_t);
