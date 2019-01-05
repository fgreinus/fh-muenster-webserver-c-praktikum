#include "utils.h"

/* change all Kapital chars to small chars.
 *
 * @param str String which should be lowercased
 */
void string_tolower(char *str) {
    if (!str) {
        return;
    }

    for (int i = 0; str[i]; i++) {
        str[i] = (char) tolower(str[i]);
    }

}

/* Sets the docroot and creates the realpath.
 *      If !docroot creates a 500 Response
 * @param response Response for which the path should be set
 */
void set_docroot(http_response *response) {

    docroot = realpath(DOCUMENT_ROOT_BASE_PATH, NULL);

    if (!docroot) {
        printf("vhost directory not found!\n");
        build_error_response(response, STATUS_500_INTERNAL_SERVER_ERROR);
        return;
    }
    intern_path = realpath(VHOST_INTERN_PATH, NULL);
    if (!intern_path) {
        printf("vhost intern directory not found!\n");
        build_error_response(response, STATUS_500_INTERNAL_SERVER_ERROR);
        return;
    }
    extern_path = realpath(VHOST_EXTERN_PATH, NULL);
    if (!extern_path) {
        printf("vhost extern directory not found!\n");
        build_error_response(response, STATUS_500_INTERNAL_SERVER_ERROR);
        return;
    }

}

/* tests if file exists and is readable
 * @param file_absolute_path
 * @return returns 0 if file is readable
 */
bool file_exists_and_is_readable(const char *file_absolute_path) {
    return access(file_absolute_path, R_OK) == 0;
}

size_t file_get_length(FILE *file_resource) {
    long initial_position = ftell(file_resource);
    fseek(file_resource, 0L, SEEK_END);
    long end_position = ftell(file_resource);
    fseek(file_resource, initial_position, SEEK_SET);

    return (size_t) end_position;
}

/* copys a string from src to dest
 *
 * @param dest destination wher the string is copied
 * @param size of the src string
 * @param src string which is copied
 * @return pointer destination
 */
char *strncpy_safe(char *dest, size_t size, char *src) {
    if (size > 0) {
        size_t i;
        for (i = 0; i < size - 1 && src[i]; i++) {
            dest[i] = src[i];
        }
        dest[i] = '\0';
    }
    return dest;
}

/* appends the string pointed to by src to the end of the string pointed to by dest.
 *
 * @param dest string where src should be appended.
 * @param src string which should be appended
 * @param dest_size max size of dest
 * @return
 */
char *strncat_safe(char *dest, const char *src, size_t dest_size) {

    size_t srclen = strlen(src);
    size_t destlen = strlen(dest);

    dest_size -= destlen + 1;

    if (!dest_size) {
        return NULL;
    }

    if (srclen > dest_size) {
        srclen = dest_size;
    }

    memcpy(dest + destlen, src, srclen);
    dest[destlen + srclen] = '\0';

    return dest;
}

/* returns the file extension.
 *  if there is none NULL will be returned
 * @param file file which extension is required
 * @return returns string with file extension
 */
char *get_file_extension(char *file) {
    file = strrchr(file, '.');

    /* file without extension */
    if (!file) {
        return NULL;
    } else {
        return file + 1;
    }
}

/* encodes url code to normal asci chars
 * Parameter is the source string
 *
 * @src source which should be converted
 */
void urltoascii(char *src) {
    char x, y, ascii;
    char *ptr = src;
    while (*src) {

        /* checks if a char is an urlcode or ascii code */
        if ((*src == '%') && ((x = src[1]) && (y = src[2])) && (isxdigit(x) && isxdigit(y))) {
            if (x >= 'a') {
                x -= 'a' - 'A';
            }
            if (x > 'A') {
                x -= ('A' - 10);
            } else { x -= '0'; }
            if (y >= 'a') {
                y -= 'a' - 'A';
            }
            if (y >= 'A') {
                y -= ('A' - 10);
            } else { y -= '0'; }

            ascii = (char) (16 * x + y);
            if (ascii == ' ') {
                *ptr = ' ';
            } else {
                *ptr = ascii;
            }
            ptr++;
            src += 3;

            /* handles + like ' '(space) */
        } else if (*src == '+') {
            *ptr++ = ' ';
            src++;

            /* no urlcode found, just copy the char */
        } else {
            *ptr++ = *src++;
        }
    }
    *ptr = '\0';
}
/*
 * checks if path is a valid directory
 * @param path
 * @return
 */
bool is_directory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

/* format: Tue, 15 Nov 1994 08:12:31 GMT
 * returns the current Time
 * @return
 */
char *get_local_time() {
    time_t timer;
    struct tm *time_info;
    timer = time(NULL);
    time_info = localtime(&timer);
    char *current_time_string = calloc(1, sizeof(char) * TIME_STRING_LENGTH);

    strftime(current_time_string, TIME_STRING_LENGTH, "%a, %d %b %Y %H:%M:%S %Z", time_info);
    return current_time_string;
}

/*
 * trims spaces in strings
 * @param str
 */
void trim_spaces(char *str) {

    char *ptr = str;
    size_t space_counter = 0;
    size_t delta = 0;

    /* trim leading */
    while (isspace((unsigned char) *ptr)) {
        space_counter++;
        ptr++;
    }

    if (space_counter) {
        delta = strlen(ptr);
        memcpy(ptr - space_counter, ptr, delta);
        ptr[delta - space_counter] = '\0';
        space_counter = 0;
    }

    /* reached end */
    if (*ptr == 0) {
        return;
    }

    ptr = str;

    /* leave single space between words */
    while (*ptr) {

        /* count while the current and next char is a space */
        while (isspace((unsigned char) *ptr) && isspace((unsigned char) *(ptr + 1))) {
            space_counter++;
            ptr++;
        }

        if (space_counter) {
            delta = strlen(ptr);
            memcpy(ptr - space_counter, ptr, delta);
            ptr[delta - space_counter] = '\0';
            space_counter = 0;
        }
        ptr++;
    }

    /* trim trailing */
    ptr = str + strlen(str) - 1;

    while (ptr > str && isspace((unsigned char) *ptr)) {
        ptr--;
    }

    /* terminate string */
    *(ptr + 1) = 0;

    return;
}