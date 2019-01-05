/*! \file utils.h
 *  \briefe some useful methods.
 */

#pragma once

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "echo_server.h"
#include "response_struct.h"

#define TIME_STRING_LENGTH 64

/*! \fn void string_tolower(char *)
 *  \brief change all Kapital chars to small chars
 *
 * @param str String which should be lowercased
 *
 */
void string_tolower(char *);

/*!\fn void set_docroot(http_response *)
 * \brief Sets the docroot and creates the realpath.
 *      If !docroot creates a 500 Response.
 * @param response Response for which the path should be set
 */
void set_docroot(http_response *);

/*! \fn bool file_exists_and_is_readable(const char *)
 * \brief tests if file exists and is readable
 * @param file_absolute_path
 * @return returns 0 if file is readable
 */
bool file_exists_and_is_readable(const char *);

/*!
 * @brief returns the length of the input File
 * @param File* File shich length is needed
 * @return size_t length of the File is returned
 */
size_t file_get_length(FILE *);

/*! \fn char *strncpy_safe(char *, size_t, char *)
 * \brief copys a string from src to dest
 *
 * @param dest destination wher the string is copied
 * @param size of the src string
 * @param src string which is copied
 * @return pointer destination
 */
char *strncpy_safe(char *, size_t, char *);

/*! \fn char *strncat_safe(char *, const char *, size_t)
 * \brief appends the string pointed to by src to the end of the string pointed to by dest.
 *
 * @param dest string where src should be appended.
 * @param src string which should be appended
 * @param dest_size max size of dest
 * @return
 */
char *strncat_safe(char *, const char *, size_t);

/*! \fn char *get_file_extension(char *)
 * \brief returns the file extension.
 *  if there is none NULL will be returned
 * @param file file which extension is required
 * @return returns string with file extension
 */
char *get_file_extension(char *);


/*! \fn void urltoascii(char *)
 * \brief encodes url code to normal asci chars
 * @param is the source string
 *
 * @src source which should be converted
 */
void urltoascii(char *);

/*! \fn bool is_directory(const char *)
 * \brief checks if path is a valid directory
 * @param path
 * @return
 */
bool is_directory(const char *);

/*! \fn void get_local_time(char *, size_t)
 *  \brief returnes the current Time
 *  format: Tue, 15 Nov 1994 08:12:31 GMT
 * @param time_string time is returned in this pointer
 * @param length max length of the timestring
 */
char *get_local_time();

/*! \fn char *trim_spaces(char *)
 * \brief trimes spaces in strings
 * @param str
 * @return
 */
void trim_spaces(char *);