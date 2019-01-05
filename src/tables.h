/*! \file tables.h
 *  \brief useful methods for working with chars.
 */

#pragma once

#include <stdbool.h>

/*! \fn bool check_method_char(char)
 *  \brief evaluates if a given char is allowed in http method
 *
 *  \param character char that is checked
 *
 *  \return bool true if character is allowed, false if not
 */
bool check_method_char(char);

/*! \fn bool check_version_char(char)
 *  \brief evaluates if a given char is allowed in http version field
 *
 *  \param character char that is checked
 *
 *  \return bool true if character is allowed, false if not
 */
bool check_version_char(char);

/*! \fn check_uri_char(char)
 *  \brief evaluates if a given char is allowed in http uri
 *
 *  \param character char that is checked
 *
 *  \return bool true if character is allowed, false if not
 */
bool check_uri_char(char);

/*! \fn check_header_name_char(char)
 *  \brief evaluates if a given char is allowed in a http header name
 *
 *  \param character char that is checked
 *
 *  \return bool true if character is allowed, false if not
 */
bool check_header_name_char(char);

/*! \fn check_header_value_char(char)
 *  \brief evaluates if a given char is allowed in a http header value
 *
 *  \param character char that is checked
 *
 *  \return bool true if character is allowed, false if not
 */
bool check_header_value_char(char);

/*!
 * @brief checks if the value of the Authorization Header is valid and only contains bas64 signs
 * @param char which is checked
 * @return bool true if the char is valid otherwise false
 */
bool check_auth_header_base64_char(char);