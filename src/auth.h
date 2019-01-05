/**
 * @file auth.h
 * @author PSE Gruppe 10
 * @date SoSe 2017
 * @brief Functions needed to provide HTTP Authorization
 */

# pragma once

#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "base64.h"
#include "echo_server.h"
#include "tables.h"

/**
 * @brief tiny macro to get the bigger of two given values and return it
 */
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/**
 * @brief struct to hold the information from parsed user input
 *        is_authorized if the user input matches the data in the <tt>htpasswd</tt>
 * @see parse_htpasswd
 */
typedef struct {
    char *username;
    char *password;
    bool is_authorized;
} credentials;

/**
 * @brief Parses the Authorization Header and builds a <tt>credentials</tt> struct to match against <tt>htpasswd</tt> entries
 * @param auth-header The Authorization Header from the request as a string
 * @return a pointer to a credentials struct with the information the client provided in his request
 * @warning As this function returns a pointer, the consumer of this function has to manually free it!
 */
credentials *parse_auth_header(char *);

/**
 * @brief Parses entries in the htpasswd file and matches them against the credentials provided in @p credentials
 *
 * @param credentials A pointer to the credentials struct with the Authorization information taken from the incoming request
 */
void parse_htpasswd(credentials *);