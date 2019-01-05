#include "response.h"

char *response_get_message_for_status_code(enum response_status_code status_code) {

    for (int i = 0; i < sizeof(messages) / sizeof(messages[0]); ++i) {
        if (messages[i].number == status_code) {
            return messages[i].message;
        }
    }

    return "";
}

char* build_error_page_path(enum response_status_code status_code)
{
    char* path = calloc(1, PATH_MAX);
    snprintf(path, PATH_MAX, "%s/%i.html", ERROR_PAGE_BASE_PATH, status_code);

    char* abs_path = NULL;
    abs_path = realpath(path, NULL);

    if (file_exists_and_is_readable(abs_path)) {
        free(path);
        return abs_path;
    } else {
        free(path);
        free(abs_path);
        return NULL;
    }
}

/*
 * initializes struct http_res with defined buffersize
 */
http_response *response_initialize() {

    http_response *response = calloc(1, sizeof(http_response));

    if (response == NULL) {
        printf("Out of memory.");
        exit(1);
    }

    response->header = calloc(1, sizeof(http_response_buffer));
    response->body = calloc(1, sizeof(http_response_buffer));

    if (response->header == NULL || response->body == NULL) {
        printf("Out of memory.");
        exit(1);
    }

    response->header->buffer = calloc(1, BUFSIZE_INITIAL);
    response->body->buffer = calloc(1, BUFSIZE_INITIAL);

    if (response->header->buffer == NULL || response->body->buffer == NULL) {
        printf("Out of memory.");
        exit(1);
    }

    response->header->size = BUFSIZE_INITIAL;
    response->body->size = BUFSIZE_INITIAL;
    response->header->end = 0;
    response->body->end = 0;

    response->resource_path = NULL;
    response->is_debug = false;
    response->is_intern = false;
    response->is_error_response = false;
    response->is_ready = false;
    response->mime_type = MIME_TYPE_OCTET_STREAM;

    return response;
}


/*
 * attaches new parts to response buffer
 * and doubles buffersize if buffer to small
 */
size_t response_buffer_attach_dynamic(http_response_buffer *buffer, char *new_part, size_t new_part_len) {

    /* test if buffer + new part to small(>= buffer->size) */
    while (buffer->end + new_part_len >= buffer->size) {
        buffer->buffer = realloc(buffer->buffer, 2 * buffer->size);
        if (buffer->buffer == NULL) {
            printf("Out of memory.");
            exit(3);
        }
        buffer->size = 2 * buffer->size;
    }
    memcpy(buffer->buffer + buffer->end, new_part, new_part_len);
    buffer->end += new_part_len;

    return new_part_len;
}

void response_reset(http_response *response) {
    response->header->buffer = realloc(response->header->buffer, BUFSIZE_INITIAL);
    response->body->buffer = realloc(response->body->buffer, BUFSIZE_INITIAL);
    if (response->body->buffer == NULL || response->header->buffer == NULL) {
        exit(1);
    }
    memset(response->header->buffer, 0, BUFSIZE_INITIAL);
    memset(response->body->buffer, 0, BUFSIZE_INITIAL);

    if (response->resource_path != NULL) {
        free(response->resource_path);
        response->resource_path = NULL;
    }

    response->header->size = BUFSIZE_INITIAL;
    response->body->size = BUFSIZE_INITIAL;
    response->header->end = 0;
    response->body->end = 0;

    response->resource_path = NULL;
    response->is_debug = false;
    response->is_intern = false;
    response->is_error_response = false;
    response->is_ready = false;
}

size_t response_load_file(http_response *response, char *file_name) {
    if (!file_name) {
        build_error_response(response, STATUS_404_NOT_FOUND);
        return 0;
    }

    if (!file_exists_and_is_readable(file_name)) {
        build_error_response(response, STATUS_404_NOT_FOUND);
        return 0;
    }

    FILE *file_resource = fopen(file_name, "r");
    if (file_resource == NULL) {
        build_error_response(response, STATUS_404_NOT_FOUND);
        return 0;
    }

    size_t file_length = file_get_length(file_resource);

    /* we alloc on a pointer, that was passed to this function - so we don't need to free it on our own */
    char *file_content = calloc(1, file_length * sizeof(char));
    if (file_content == NULL) {
        printf("Failed allocating %lu bytes of memory.", file_length);
        fclose(file_resource);
        return 0;
    }

    fseek(file_resource, 0, SEEK_SET);
    fread(file_content, 1, file_length, file_resource);
    fclose(file_resource);

    size_t content_length = response_buffer_attach_dynamic(response->body, file_content, file_length);
    free(file_content);

    return content_length;
}

http_response *response_create(enum response_status_code status_code) {
    http_response *response = response_initialize();

    response->is_error_response = false;
    response->is_ready = false;
    response->status_code = status_code;

    return response;
}

void response_build_header(http_response *response, enum response_status_code status_code) {
    char *line_separator = "\r\n";
    char *http_line_key = "HTTP/1.1 ";
    char *header_body_seperator = "\r\n\r\n";
    char *content_length_key = "Content-Length: ";
    char *content_type_key = "Content-Type: ";
    char *server_line = "Server: Gruppe 10 Webserver";
    char *date_header_key = "Date: ";
    char *connection_close_header = "Connection: close";
    char *current_time_string = get_local_time();

    /* convert the status code (int) to a string */
    char status_code_as_string[20] = "";
    snprintf(status_code_as_string, sizeof(status_code_as_string), "%i ", status_code);
    char *status_code_message = response_get_message_for_status_code(status_code);

    /* fill in http_line_key-line */
    response_buffer_attach_dynamic(response->header, http_line_key, strlen(http_line_key));
    response_buffer_attach_dynamic(response->header, status_code_as_string, strlen(status_code_as_string));
    response_buffer_attach_dynamic(response->header, status_code_message, strlen(status_code_message));
    /* new line */
    response_buffer_attach_dynamic(response->header, line_separator, strlen(line_separator));

    /* fill in content-type */
    char *mime_type_text = resolve_mime_type_identifier(response->mime_type);
    response_buffer_attach_dynamic(response->header, content_type_key, strlen(content_type_key));
    response_buffer_attach_dynamic(response->header, mime_type_text, strlen(mime_type_text));
    /* new line */
    response_buffer_attach_dynamic(response->header, line_separator, strlen(line_separator));

    /* current date */
    response_buffer_attach_dynamic(response->header, date_header_key, strlen(date_header_key));
    response_buffer_attach_dynamic(response->header, current_time_string, strlen(current_time_string));
    free(current_time_string);
    /* new line */
    response_buffer_attach_dynamic(response->header, line_separator, strlen(line_separator));

    /* server line */
    response_buffer_attach_dynamic(response->header, server_line, strlen(server_line));
    /* new line */
    response_buffer_attach_dynamic(response->header, line_separator, strlen(line_separator));

    /* connection close line */
    response_buffer_attach_dynamic(response->header, connection_close_header, strlen(connection_close_header));
    /* new line */
    response_buffer_attach_dynamic(response->header, line_separator, strlen(line_separator));

    /* fill in content-length string (body has to exist to determine the content-length) */
    response_buffer_attach_dynamic(response->header, content_length_key, strlen(content_length_key));
    char content_length[256] = "";
    snprintf(content_length, sizeof(content_length), "%zu", response->body->end);
    response_buffer_attach_dynamic(response->header, content_length, strlen(content_length));

    if (status_code == STATUS_401_UNAUTHORIZED) {
        /* new line */
        response_buffer_attach_dynamic(response->header, line_separator, strlen(line_separator));
        char *www_authenticate_header = "WWW-Authenticate: Basic realm=\"Gruppe 10\"";
        response_buffer_attach_dynamic(response->header, www_authenticate_header, strlen(www_authenticate_header));
    }

    /* put in the header->body separator */
    response_buffer_attach_dynamic(response->header, header_body_seperator, strlen(header_body_seperator));
}

void build_debug_response(request_header *header, http_response *response) {
    enum response_status_code status_code = STATUS_200_OK;

    response->status_code = status_code;
    response->mime_type = MIME_TYPE_TEXT_HTML;

    char *debug_first_part = "<html>\n"
            " <body>\n"
            "  <h1>Willkommen im externen Bereich!</h1>\n"
            "  <pre> \n"
            "HTTP-Methode: ";
    char *debug_second_part = " \n"
            "HTTP-Ressource: ";

    char *debug_third_part = " \n"
            "HTTP-Protokoll-Version: ";

    char *debug_fourth_part = " \n"
            "  </pre>\n"
            " </body>\n"
            "</html>";

    response_buffer_attach_dynamic(response->body, debug_first_part, strlen(debug_first_part));
    response_buffer_attach_dynamic(response->body, header->method, strlen(header->method));
    response_buffer_attach_dynamic(response->body, debug_second_part, strlen(debug_second_part));
    response_buffer_attach_dynamic(response->body, header->uri, strlen(header->uri));
    response_buffer_attach_dynamic(response->body, debug_third_part, strlen(debug_third_part));
    response_buffer_attach_dynamic(response->body, header->protocol, strlen(header->protocol));
    response_buffer_attach_dynamic(response->body, debug_fourth_part, strlen(debug_fourth_part));

    response_build_header(response, status_code);

    response->is_ready = true;
}

void build_error_response(http_response *response, enum response_status_code status_code) {
    if (response == NULL) {
        response = response_initialize();
    } else {
        response_reset(response);
    }

    response->status_code = status_code;
    response->mime_type = MIME_TYPE_TEXT_HTML;


    char *error_page_path = build_error_page_path(status_code);
    if (error_page_path == NULL) {
        char* fallback_page = "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
                "<html>\n"
                "<head>\n"
                "    <title>500 Internal Server Error</title>\n"
                "</head>\n"
                "<body>\n"
                "<h1>500 - Internal Server Error</h1>\n"
                "<hr>\n"
                "<address>Gruppe 10 Webserver</address>\n"
                "</body>\n"
                "</html>";
        response_buffer_attach_dynamic(response->body, fallback_page, strlen(fallback_page));
    } else {
        response_load_file(response, error_page_path);
        free(error_page_path);
    }

    response_build_header(response, status_code);

    response->is_ready = true;
    response->is_error_response = true;
}

void process_request_to_response(request_header *request, http_response *response) {
    if (response->is_debug) {
        build_debug_response(request, response);
    } else if (docroot && intern_path && extern_path) {
        response_load_file(response, response->resource_path);

        char *file_extension = get_file_extension(response->resource_path);
        response->mime_type = match_mime_type(file_extension);

        response_build_header(response, response->status_code);
    }

    response->is_ready = true;
}

void free_response(http_response *response) {
    free(response->header->buffer);
    free(response->body->buffer);
    free(response->header);
    free(response->body);
    free(response->resource_path);
    free(response);
}
