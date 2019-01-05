#include "mime.h"
/*matching mime_type
 *
 * The mime_type gets selected.mime_types can be text,audio video....
 * @param ext
 * @return
 */
enum mime_type match_mime_type(char *ext) {

    if (!ext) {
        return MIME_TYPE_OCTET_STREAM;
    }

    string_tolower(ext);

    for (int i = 0; i < sizeof(mime_table) / sizeof(mime_table[0]); ++i) {
        if (strcmp(mime_table[i].ext, ext) == 0) {
            return mime_table[i].identifier;
        }
    }

    /* default is "application/octet-stream" if unknown */
    return MIME_TYPE_OCTET_STREAM;
}
/*resolving mime_type
 *
 * mime_type gets identified and resolved.
 * @param identifier
 * @return
 */
char *resolve_mime_type_identifier(enum mime_type identifier) {

    for (int i = 0; i < sizeof(mime_table) / sizeof(mime_table[0]); ++i) {
        if (mime_table[i].identifier == identifier) {
            return mime_table[i].type;
        }
    }

    return "application/octet-stream";
}

