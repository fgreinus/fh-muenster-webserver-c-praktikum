#include "header.h"

enum header_field match_header_field(char *name, size_t length) {

    if (!name) {
        return HEADER_UNKNOWN;
    }

    for (int i = 0; i < sizeof(header_field) / sizeof(header_field[0]); ++i) {
        if (strncasecmp(header_field[i].name, name, length) == 0) {
            return header_field[i].identifier;
        }
    }

    /* default is unknown and ignored */
    return HEADER_UNKNOWN;

}
