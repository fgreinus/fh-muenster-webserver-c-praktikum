#include "request.h"

void parse_request(char *buffer_in, request_header *req_header, http_response *response) {

    char *buffer_parse_method = calloc(1, BUFFER_SIZE);
    char *buffer_parse_header = NULL;
    char *parser = NULL;
    char *validator = NULL;

    char *header = calloc(1, BUFFER_SIZE);
    char *header_field_name_ptr = NULL;
    char *header_field_content_ptr = NULL;
    size_t method_length = 0;
    size_t header_length = 0;
    size_t header_field_name_length = 0;
    size_t header_field_content_length = 0;
    size_t uri_length = 0;


    if (!buffer_parse_method) {
        build_error_response(response, STATUS_500_INTERNAL_SERVER_ERROR);
        return;
    }

    if (!header) {
        build_error_response(response, STATUS_500_INTERNAL_SERVER_ERROR);
        return;
    }

    if (!allocate_header(req_header)) {
        free(buffer_parse_method);
        free(header);
        build_error_response(response, STATUS_500_INTERNAL_SERVER_ERROR);
        return;
    }

    memcpy(buffer_parse_method, buffer_in, BUFFER_SIZE);

    /* check if CR exists */
    if (!strchr(buffer_parse_method, '\r')) {
        free(header);
        free(buffer_parse_method);
        build_error_response(response, STATUS_400_BAD_REQUEST);
        return;
    }

    /* check if LF exists */
    if (!strchr(buffer_parse_method, '\n')) {
        free(header);
        free(buffer_parse_method);
        build_error_response(response, STATUS_400_BAD_REQUEST);
        return;
    }

    /* parse method
     * if an invalid char is found, stop parsing and send error response
     * first check if first char is not LF or CR
     */
    if ((strcspn(buffer_parse_method, "\n") == 0 || strcspn(buffer_parse_method, "\r")) == 0) {
        free(header);
        free(buffer_parse_method);
        build_error_response(response, STATUS_400_BAD_REQUEST);
        return;
    }

    parser = strtok(buffer_parse_method, "\r\n");

    method_length = strcspn(parser, " ");

    if (!strchr(parser, ' ')) {
        free(header);
        free(buffer_parse_method);
        build_error_response(response, STATUS_400_BAD_REQUEST);
        return;
    }

    /* ignore unknown method for now
     * check if method has invalid chars
     */
    validator = parser;
    for (int i = 0; i < method_length; ++i) {
        if (!check_method_char(validator[0])) {

            /* bad method char */
            free(header);
            free(buffer_parse_method);
            build_error_response(response, STATUS_400_BAD_REQUEST);
            return;
        }
        validator++;
    }

    if (method_length >= HEADER_VALUE_SIZE) {
        method_length = HEADER_VALUE_SIZE - 1;
    }
    memcpy(req_header->method, parser, method_length);


    /* go to uri, assume everything up to last ' ' is for uri */
    parser = strchr(parser, ' ');

    /* no space after method found */
    if (!parser) {
        free(header);
        free(buffer_parse_method);
        build_error_response(response, STATUS_400_BAD_REQUEST);
        return;
    }

    /* go to begin of uri */
    parser++;
    validator = (strrchr(parser, ' '));
    if (!validator) {
        free(header);
        free(buffer_parse_method);
        build_error_response(response, STATUS_400_BAD_REQUEST);
        return;
    }
    uri_length = strlen(parser) - strlen(strrchr(parser, ' '));

    /* uri has to begin with '/' */
    if (strncmp(parser, "/", 1) == 0) {
        validator = parser;

        /* check if uri has invalid chars */
        for (int i = 0; i < uri_length; ++i) {
            if (!check_uri_char(validator[0])) {

                /* bad uri char */
                free(header);
                free(buffer_parse_method);
                build_error_response(response, STATUS_400_BAD_REQUEST);
                return;
            }
            validator++;
        }
        if (uri_length >= BUFFER_SIZE) {
            uri_length = BUFFER_SIZE - 1;
        }
        memcpy(req_header->uri, parser, uri_length);
    } else {

        /* bad uri */
        free(header);
        free(buffer_parse_method);
        build_error_response(response, STATUS_400_BAD_REQUEST);
        return;
    }

    /* assume that everything after uri is for version */
    parser = strchr(parser, ' ') + 1;
    header_field_content_length = strcspn(parser, "\r\n");
    validator = parser;

    /* check if version has invalid chars */
    while (*validator != '\0') {
        if (!check_version_char(validator[0])) {

            /* bad header char */
            free(header);
            free(buffer_parse_method);
            build_error_response(response, STATUS_400_BAD_REQUEST);
            return;
        }
        validator++;
    }

    /* detect "HTTP/0.9", "HTTP/1.0", "HTTP/1.1" */
    if ((strncmp(parser, "HTTP/0.9", header_field_content_length) == 0) ||
        (strncmp(parser, "HTTP/1.0", header_field_content_length) == 0) ||
        (strncmp(parser, "HTTP/1.1", header_field_content_length) == 0)) {
        if (header_field_content_length >= HEADER_VALUE_SIZE) {
            header_field_content_length = HEADER_VALUE_SIZE - 1;
        }
        memcpy(req_header->protocol, parser, header_field_content_length);
    } else {
        /* bad protocol */
        free(header);
        free(buffer_parse_method);
        build_error_response(response, STATUS_400_BAD_REQUEST);
        return;
    }

    free(buffer_parse_method);
    buffer_parse_header = calloc(1, BUFFER_SIZE);
    memcpy(buffer_parse_header, buffer_in, BUFFER_SIZE);

    /* jump to second line of the request */
    parser = strchr(buffer_parse_header, '\n') + 1;

    /* parse headers */
    while (parser) {
        header_length = strcspn(parser + 1, "\n");
        memcpy(header, parser, header_length);
        header_field_name_ptr = header;
        header_field_content_ptr = header;

        /* no delimiter found, either reached end of request or a bad request */
        if (!strchr(header, ':')) {
            break;
        }

        parser = strchr(parser, ':');

        /* no whitespace after colon */
        if (*(parser + 1) != ' ') {
            free(header);
            free(buffer_parse_header);
            build_error_response(response, STATUS_400_BAD_REQUEST);
            return;
        }

        /* go to header value */
        parser += 2;

        header_field_name_length = strcspn(header_field_name_ptr + 1, " ");
        header_field_content_ptr = strchr(header_field_content_ptr, ' ') + 1;
        validator = header_field_name_ptr;

        /* check if header name has invalid chars */
        for (int i = 0; i < header_field_name_length; ++i) {
            if (!check_header_name_char(validator[0])) {

                /* bad header name */
                free(header);
                free(buffer_parse_header);
                build_error_response(response, STATUS_400_BAD_REQUEST);
                return;
            }
            validator++;
        }

        validator = header_field_content_ptr;

        /* trim leading and trailing whitespaces
         * also replace multiple spaces between words with single space */
        trim_spaces(validator);
        header_field_content_length = strcspn(validator, "\r\n");

        /* check if header value has invalid chars */
        while (*validator != '\0') {
            if (!check_header_value_char(validator[0])) {

                /* bad header value */
                free(header);
                free(buffer_parse_header);
                build_error_response(response, STATUS_400_BAD_REQUEST);
                return;
            }
            validator++;
        }

        if (header_field_content_length >= HEADER_VALUE_SIZE) {
            header_field_content_length = HEADER_VALUE_SIZE - 1;
        }

        /* ignore valid, but unknown header */
        switch (match_header_field(header_field_name_ptr, header_field_name_length)) {
            case HEADER_HOST:
                memcpy(req_header->host, header_field_content_ptr, header_field_content_length);
                break;
            case HEADER_USER_AGENT:
                memcpy(req_header->user_agent, header_field_content_ptr, header_field_content_length);
                break;
            case HEADER_ACCEPT:
                memcpy(req_header->accept, header_field_content_ptr, header_field_content_length);
                break;
            case HEADER_CONNECTION:
                memcpy(req_header->connection, header_field_content_ptr, header_field_content_length);
                break;
            case HEADER_CONTENT_LENGTH:
                memcpy(req_header->content_length, header_field_content_ptr, header_field_content_length);
                break;
            case HEADER_AUTHORIZATION:
                memcpy(req_header->authorization, header_field_content_ptr, header_field_content_length);
                break;
            default:
                break;
        }

        memset(header, 0, BUFFER_SIZE);
        parser = strchr(parser, '\n');

        /* no LF after parsing */
        if (!parser) {
            free(header);
            free(buffer_parse_header);
            build_error_response(response, STATUS_400_BAD_REQUEST);
            return;
        }

        /* go to next char */
        parser++;
    }

    /* check if we have "\r\n"; this means we parsed all headers,
     * and request has empty line before entity body
     */
    if ((strncmp(parser, "\r\n", strlen("\r\n")) != 0)) {
        free(header);
        free(buffer_parse_header);
        build_error_response(response, STATUS_400_BAD_REQUEST);
        return;
    }

    validate_request(req_header, response);
    free(header);
    free(buffer_parse_header);
}

bool allocate_header(request_header *header) {
    header->method = calloc(1, HEADER_VALUE_SIZE);
    header->uri = calloc(1, BUFFER_SIZE);
    header->protocol = calloc(1, HEADER_VALUE_SIZE);
    header->host = calloc(1, HEADER_VALUE_SIZE);
    header->user_agent = calloc(1, HEADER_VALUE_SIZE);
    header->accept = calloc(1, HEADER_VALUE_SIZE);
    header->connection = calloc(1, HEADER_VALUE_SIZE);
    header->content_length = calloc(1, HEADER_VALUE_SIZE);
    header->authorization = calloc(1, HEADER_VALUE_SIZE);

    if (header->method == NULL || header->uri == NULL ||
        header->protocol == NULL || header->host == NULL ||
        header->user_agent == NULL || header->accept == NULL ||
        header->connection == NULL || header->content_length == NULL ||
        header->authorization == NULL) {
        return false;
    }
    return true;
}

void free_header(request_header *header) {
    free(header->method);
    free(header->uri);
    free(header->protocol);
    free(header->host);
    free(header->accept);
    free(header->user_agent);
    free(header->connection);
    free(header->content_length);
    free(header->authorization);
    free(header);
}

void validate_request(request_header *header, http_response *response) {

    char *ptr = NULL;
    const char *vhost = NULL;
    const char *intern_name = VHOST_INTERN_NAME;
    const char *extern_name = VHOST_EXTERN_NAME;
    credentials *creds = NULL;

    /* detect "GET" */
    if (strncmp(header->method, "GET", strlen(header->method)) != 0) {
        build_error_response(response, STATUS_501_NOT_IMPLEMENTED);
        return;
    }

    /* check if host exists */
    if (!strlen(header->host)) {
        build_error_response(response, STATUS_400_BAD_REQUEST);
        return;
    }

    ptr = header->host;
    while (*ptr) {
        if (*ptr == ':') {
            *ptr = '\0';
            break;
        } else {
            ++ptr;
        }
    }

    /* check if host is "intern" */
    if ((strlen(header->host)) && (strncasecmp(header->host, VHOST_INTERN_NAME, strlen(header->host)) == 0)) {
        response->is_intern = true;
        vhost = intern_name;
    } else {
        vhost = extern_name;
    }

    /* need auth */
    if (!strlen(header->authorization) && response->is_intern) {
        build_error_response(response, STATUS_401_UNAUTHORIZED);
        return;

        /* has already auth header */
    } else if (response->is_intern) {

        /* bad user input */
        creds = parse_auth_header(header->authorization);
        if (!creds) {
            build_error_response(response, STATUS_401_UNAUTHORIZED);
            return;
        }

        /* check if authorized, store result in struct */
        parse_htpasswd(creds);
        free(creds->username);
        free(creds->password);
        if (!creds->is_authorized) {
            free(creds);
            build_error_response(response, STATUS_401_UNAUTHORIZED);
            return;
        }
        free(creds);
    }

    /* check if content_length is a valid integer */
    if (strlen(header->content_length)) {
        ptr = header->content_length;
        while (*ptr) {
            if (!isdigit(*ptr)) {
                build_error_response(response, STATUS_400_BAD_REQUEST);
                return;
            } else {
                ++ptr;
            }
        }
    }

    urltoascii(header->uri);

    /* check if url has '%' after decoding, this is not allowed */
    ptr = header->uri;
    while (*ptr) {
        if (*ptr == '%') {
            build_error_response(response, STATUS_400_BAD_REQUEST);
            return;
        } else {
            ++ptr;
        }
    }

    build_absolute_path(header, response, vhost);
}

void build_absolute_path(request_header *req_header, http_response *response, const char *vhost) {
    char *abs_path = NULL;
    char *jail = NULL;

    /* at this state, we know that the uri as only valid characters and begins with '/'
     * check if /debug is called
     */
    if ((strcmp(req_header->uri, DEBUG_URI) == 0) && (strlen(req_header->uri) == strlen(DEBUG_URI))) {
        response->is_debug = true;
        return;
    }

    /* uri too long (in request) */
    if (strlen(req_header->uri) + strlen(docroot) + 2 > PATH_MAX) {
        build_error_response(response, STATUS_400_BAD_REQUEST);
        return;
    }

    abs_path = calloc(1, PATH_MAX);
    if (!abs_path) {
        build_error_response(response, STATUS_500_INTERNAL_SERVER_ERROR);
        return;
    }

    /* check if docroot ends with '/' */
    if (docroot[strlen(docroot) - 1] != '/') {
        strncpy_safe(abs_path, PATH_MAX, docroot);
        abs_path = strncat_safe(abs_path, "/", PATH_MAX);
    }

    /* if this failed, error out */
    if (!abs_path) {
        build_error_response(response, STATUS_400_BAD_REQUEST);
        free(abs_path);
        return;
    }

    /* append vhost */
    abs_path = strncat_safe(abs_path, vhost, PATH_MAX);

    /* if this failed, error out */
    if (!abs_path) {
        build_error_response(response, STATUS_400_BAD_REQUEST);
        free(abs_path);
        return;
    }

    /* check if abs_path ends with '/' */
    if (abs_path[strlen(docroot) - 1] != '/') {
        abs_path = strncat_safe(abs_path, "/", PATH_MAX);
    }

    /* if this failed, error out */
    if (!abs_path) {
        build_error_response(response, STATUS_400_BAD_REQUEST);
        free(abs_path);
        return;
    }

    /* append uri (excluding '/' from uri) */
    abs_path = strncat_safe(abs_path, req_header->uri + 1, PATH_MAX);

    /* path too long after appending */
    if (!abs_path) {
        build_error_response(response, STATUS_400_BAD_REQUEST);
        free(abs_path);
        return;
    }

    /* check if we are still in our document root */
    jail = realpath(abs_path, NULL);

    /* file / folder not existing */
    if (!jail) {
        build_error_response(response, STATUS_404_NOT_FOUND);
        free(abs_path);
        return;
    }

    /* case intern */
    if (response->is_intern) {

        /* traversing out of intern */
        if (strncmp(intern_path, jail, strlen(intern_path))) {
            build_error_response(response, STATUS_404_NOT_FOUND);
            free(jail);
            free(abs_path);
            return;
        }
    } else {

        /* traversing out of extern */
        if (strncmp(extern_path, jail, strlen(extern_path))) {
            build_error_response(response, STATUS_404_NOT_FOUND);
            free(jail);
            free(abs_path);
            return;
        }
    }

    /* file / folder outside document root, traversing not allowed */
    if (strncmp(docroot, jail, strlen(docroot))) {
        build_error_response(response, STATUS_404_NOT_FOUND);
        free(jail);
        free(abs_path);
        return;
    }

    /* if path does not end with '/', check if it is a file or directory */
    if (jail[strlen(jail) - 1] != '/') {
        if (is_directory(jail)) {

            /* is a directory, point to index.html */
            jail = strncat_safe(jail, "/index.html", PATH_MAX);
        }

        /* is a directory, point to index.html */
    } else {
        abs_path = strncat_safe(abs_path, "index.html", PATH_MAX);
    }

    /* path too long after appending */
    if (!abs_path) {
        build_error_response(response, STATUS_400_BAD_REQUEST);
        free(jail);
        free(abs_path);
        return;
    }
    response->resource_path = jail;
    free(abs_path);
}