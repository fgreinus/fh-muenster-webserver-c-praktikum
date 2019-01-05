/* Version: 2.1.1 */
#include <errno.h>      /* errno */
#include <netinet/ip.h> /* ^ */
#include <signal.h>     /* sigaction, struct sigaction, siginfo_t, SIGTERM */
#include <stdbool.h>    /* bool */
#include <stdio.h>      /* fprintf */
#include <stdlib.h>     /* exit, malloc, free */
#include <string.h>     /* memset, strerror */
#include <sys/socket.h> /* struct sockaddr_in, socket, setsockopt, bind, listen, socklen_t, accept */
#include <unistd.h> /* read, write, close */
#include "echo_server.h"


#define PORT 31337
#define BUFFER_SIZE 8192

/**
 * Globale Variablen.
 */
static bool run = true;

/**
 * Fehlerbehandlung.
 */
static void error(char *msg) {
    fprintf(stderr, "%s", msg);
    if (errno) {
        fprintf(stderr, ", errno: %s", strerror(errno));
    }
    fprintf(stderr, "\n");
    exit(1);
}

/**
 * Behandlung des SIGINT-Signals (Strg+C) um den Server zu beenden.
 */
static void handle_signal(int signum) {
    if (signum != SIGINT) {
        error("ERROR unexpected signal");
    }

    /*
     * Beende den Server nach dem Abarbeiten des letzten Clients.
     */
    run = false;
}

/**
 * Registriert das SIGINT-Signal (Strg+C) um den Server beenden zu können.
 */
static void register_signal() {
    struct sigaction action;

    /*
     * Konfigurieren des Signal-Handlers.
     */
    memset(&action, 0, sizeof(action));
    action.sa_handler = handle_signal;

    /*
     * Registrierung des Signal-Handlers.
     */
    if (sigaction(SIGINT, &action, NULL) < 0) {
        error("ERROR registering signal handler");
    }
}

/**
 * Erstellt und konfiguriert den Netzwerk-Socket, über den die Verbindungen
 * angenommen werden.
 */
static int setup_socket() {
#ifdef STDIN_ONLY
    return STDOUT_FILENO;
#endif

    int opt = 1;
    int sockfd = 0;
    struct sockaddr_in serv_addr;

    /*
     * Setzt Konfigurationsvariablen für den Socket, z.B. die Portnummer.
     */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    /*
     * Erstelle den Socket.
     */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    /*
     * Verwende den Socket, selbst wenn er aus einer vorigen Ausführung
     * im TIME_WAIT Status ist.
     */
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt,
                   sizeof(int)) < 0)
        error("ERROR on setsockopt");

    /*
     * Melde, dass der Socket eingehende Verbindungen akzeptieren soll.
     */
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    /*
     * Horche auf dem Socket nach eingehenden Verbindungen. Es werden maximal
     * fünf gleichzeitige Verbindungen erlaubt.
     */
    if (listen(sockfd, 5) < 0) {
        error("listen");
    }
    return sockfd;
}

/**
 * Die Hauptschleife, in der eingehende Verbindungen angenommen werden.
 */
static void main_loop(int sockfd) {
    int newsockfd;
    ssize_t length;

#ifndef STDIN_ONLY
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
#endif

    void *const buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        error("ERROR at malloc.");
    }

    build_decoding_table1();

    /*
     * Die Hauptschleife des Programms.
     */
    while (run) {

#ifndef STDIN_ONLY
        /*
         * Der accept()-Aufruf blockiert, bis eine neue Verbindung rein kommt.
         */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0) {
            /*
             * Wenn der Server mit dem SIGINT-Signal beendet wird, schlägt accept
             * mit EINTR (interrupted) fehl.
             */
            if (errno == EINTR) {
                break;
            }
            error("ERROR on accept");
        }
#else
        newsockfd = STDIN_FILENO;
#endif

        /*
         * Lies die ankommenden Daten von dem Socket in das Array buffer.
         */
        memset(buffer, 0, BUFFER_SIZE);
        length = read(newsockfd, buffer, BUFFER_SIZE - 1);
        if (length < 0) {
            if (errno == EINTR) {
                break;
            }
            error("ERROR reading from socket");
        }

        http_response *response = response_create(STATUS_200_OK);

        /* calculate docroot on every request in case it gets deleted */
        set_docroot(response);

        /* Parse Request */
        request_header *header = calloc(1, sizeof(request_header));
        if (docroot && intern_path && extern_path){
            parse_request(buffer, header, response);
        }

        /* if we have not already filled the response, do it now (this is usually the case when everything went good */
        if (!response->is_ready) {
            process_request_to_response(header, response);
        }


/*
 * Schreibe die ausgehenden Daten auf den Socket.
 */
#ifndef STDIN_ONLY
        /* concatenate header and body together and output it in one write operation */
        char *output = calloc(1, response->header->end + response->body->end + 1);
        memcpy(output, response->header->buffer, response->header->end);
        memcpy(output + response->header->end, response->body->buffer, response->body->end);
        length = write(newsockfd, output, response->header->end + response->body->end);
        if (length < 0) {
            /* in case we were not able to write to the socket we need to stop the current loop and free everything */
            free(output);
            free_response(response);
            free_header(header);
            free(docroot);
            free(extern_path);
            free(intern_path);
            close(newsockfd);

            continue;
        }

#else
        /*
     * Gib die eingegangenen Daten auf der Kommandozeile aus.
     */
    if (write(STDOUT_FILENO, buffer, length) < 0) {
      error("ERROR writing to STDOUT");
    }
#endif

        free_response(response);
        free_header(header);
        free(docroot);
        free(extern_path);
        free(intern_path);
        free(output);

/*
 * Schließe die Verbindung.
 */
#ifndef STDIN_ONLY
        if (close(newsockfd) < 0) {
            error("ERROR on close");
        }
#endif
    }

    /*
     * Lösche den Buffer und schließe den Socket. Dieser Aufruf sollte wegen der
     * Endlosschleife niemals ausgeführt werden.
     */
    base64_cleanup();
    free(buffer);
#ifndef STDIN_ONLY
    if (close(sockfd) < 0) {
        error("ERROR on close");
    }
#endif
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    register_signal();
    const int sockfd = setup_socket();
    main_loop(sockfd);

    return 0;
}
