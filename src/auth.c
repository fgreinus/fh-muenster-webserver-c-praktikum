#include "auth.h"

credentials *parse_auth_header(char *auth_header_value) {

    char *encoded_creds = NULL;
    char *encoded_creds_validator = NULL;
    unsigned char *decoded = NULL;
    char *decoded_as_string = NULL;
    char *colon_ptr = NULL;
    credentials *creds = NULL;
    size_t encoded_length = 0;
    size_t decoded_length = 0;
    size_t username_length = 0;
    size_t password_length = 0;

    /* We only accept Basic Authorization */
    if (strncasecmp(auth_header_value, "basic ", strlen("basic ")) != 0) {
        return NULL;
    }

    encoded_creds = encoded_creds_validator = strchr(auth_header_value, ' ') + 1;
    encoded_length = strlen(encoded_creds);

    /* check characters in header after 'Basic ' */
    for (int i = 0; i < encoded_length; ++i) {

        /* not base64 char */
        if (!check_auth_header_base64_char(encoded_creds_validator[0])) {
            return NULL;
        }
        encoded_creds_validator++;
    }

    decoded = base64_decode(encoded_creds, strlen(encoded_creds), &decoded_length);

    decoded_as_string = calloc(decoded_length + 1, sizeof(char));
    memcpy(decoded_as_string, decoded, decoded_length);

    colon_ptr = strchr(decoded_as_string, ':');

    if (colon_ptr != NULL && (colon_ptr == strrchr(decoded_as_string, ':'))) {

        username_length = (size_t) (colon_ptr - decoded_as_string);
        password_length = decoded_length - username_length - 1;

        creds = calloc(1, sizeof(credentials));

        creds->username = calloc(1, HEADER_VALUE_SIZE);
        creds->password = calloc(1, HEADER_VALUE_SIZE);

        memcpy(creds->username, decoded_as_string, username_length);
        memcpy(creds->password, decoded_as_string + username_length + 1, password_length);

        /* print credentials to stdout */
        printf("Benutzer: %s\nPassword: %s\n\n", creds->username, creds->password);

        free(decoded);
        free(decoded_as_string);

        return creds;

    } else {

        free(decoded);
        free(decoded_as_string);
        return NULL;
    }
}

void parse_htpasswd(credentials *creds) {

    FILE *htpasswd;
    char htpasswd_line[BUFFER_SIZE];
    char *encoded = NULL;
    char *colon_ptr = NULL;
    char *linefeed_ptr = NULL;
    char *htpasswd_base64_ptr = NULL;
    unsigned char hash[SHA_DIGEST_LENGTH];
    size_t htpasswd_username_length = 0;
    size_t encoded_length = 0;

    /* file not found or not readable */
    if (!file_exists_and_is_readable(HTPASSWD_PATH)) {
        return;
    }

    htpasswd = fopen(HTPASSWD_PATH, "r");

    /* parse htpasswd line for line */
    while (fgets(htpasswd_line, BUFFER_SIZE, htpasswd)) {
        colon_ptr = strchr(htpasswd_line, ':');

        /* htpasswd has no colon in this line */
        if (!colon_ptr) {
            fclose(htpasswd);
            return;
        }

        htpasswd_username_length = (size_t) (colon_ptr - htpasswd_line);

        /* check if username matches */
        if (strncmp(htpasswd_line, creds->username, MAX(htpasswd_username_length, strlen(creds->username))) == 0) {

            /* after username is the hash specified */
            if (strncmp(colon_ptr, ":{SHA}", strlen(":{SHA}")) != 0) {
                fclose(htpasswd);
                return;
            }

            htpasswd_base64_ptr = linefeed_ptr = colon_ptr + (strlen(":{SHA}"));

            /* remove LF from string */
            while (*linefeed_ptr) {
                if (*linefeed_ptr == '\n') {
                    *linefeed_ptr = '\0';
                    break;
                } else {
                    ++linefeed_ptr;
                }

            }

            /* hash password that was given by request */
            SHA1((const unsigned char *) creds->password, strlen(creds->password), hash);

            /* encode hashed password */
            encoded = base64_encode(hash, SHA_DIGEST_LENGTH, &encoded_length);

            /* failure on encoding */
            if (!encoded) {
                fclose(htpasswd);
                return;
            }

            /* compare with base64 string in htpasswd */
            if (strncmp(htpasswd_base64_ptr, encoded, MAX(encoded_length, strlen(htpasswd_base64_ptr))) != 0) {
                fclose(htpasswd);
                free(encoded);
                return;
            }

            /* everything ok */
            creds->is_authorized = true;
            fclose(htpasswd);
            free(encoded);
            return;
        }
    }

    /* reached end of file */
    fclose(htpasswd);
}