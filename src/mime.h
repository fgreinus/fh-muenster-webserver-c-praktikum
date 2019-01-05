/*! \file mime.h
 *  \brief mimes.
 */
# pragma once

#include "mime_types.h"
#include "utils.h"

/*! \var static const struct mime_table[]
 * \brief mime types we support
 *
 */
static const struct {
    char *ext;
    char *type;
    enum mime_type identifier;
} mime_table[] = {
        {"css",  "text/css",        MIME_TYPE_TEXT_CSS},
        {"htm",  "text/html",       MIME_TYPE_TEXT_HTML},
        {"html", "text/html",       MIME_TYPE_TEXT_HTML},
        {"jpg",  "image/jpeg",      MIME_TYPE_IMAGE_JPEG},
        {"jpeg", "image/jpeg",      MIME_TYPE_IMAGE_JPEG},
        {"gif",  "image/gif",       MIME_TYPE_IMAGE_GIF},
        {"png",  "image/png",       MIME_TYPE_IMAGE_PNG},
        {"js",   "text/javascript", MIME_TYPE_TEXT_JAVASCRIPT}
};
 /*! \fn enum mime_type match_mime_type(char *)
  * \brief checks if extension ext is supported and matches it to an enum
  *
  * \param ext extension of a file
  * @return returns the fitting mime type
  */
enum mime_type match_mime_type(char *);

/*! \fn char *resolve_mime_type_identifier(enum mime_type)
 * \brief mime_type gets identified and matched with the belonging mimestring.
 *
 * \param identifier mime type which is resolved
 * @return char* returns the matched mimestring
 */

char *resolve_mime_type_identifier(enum mime_type);
