#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/*
 CGI standard compliant program to return the client address and current local time.
 Can be used with any CGI-enabled httpd to setup a lightweight MyIP service.
*/


/* Handle general errors as Status 500 */
void internal_server_error() {
    printf("Status: 500 Internal Server Error\r\n");
    exit(5);
}


/* RFC 3875 section 6.1 requires to check for accepted request methods. */
void allow_get_request() {
    const char *method = getenv("REQUEST_METHOD");
        
    if (method == NULL) {
        internal_server_error();
    }

    if (strcmp(method, "GET") != 0) {
        printf(
            "Status: 405 Method Not Allowed\r\n"
            "Allow: GET\r\n"
            );
        exit(4);
    }
}


/* Get current local time in RFC 2822 format. */
void get_current_time(char *timebuf, size_t bufsize) {
    const time_t current_time = time(NULL);
    const struct tm *timeinfo = localtime(&current_time);

    if (timeinfo == NULL) {
        internal_server_error();
    }

    if (strftime(timebuf, bufsize, "%a, %d %b %Y %T %z", timeinfo) == 0) {
        internal_server_error();
    }
}


int main() {
    allow_get_request();

    /* get server time */
    char timebuf[64];
    get_current_time(timebuf, sizeof(timebuf));

    /* get client address */
    const char *remote_addr = getenv("REMOTE_ADDR");

    if (remote_addr == NULL) {
        internal_server_error();
    }

    printf(
        /* "Status: 200 OK\r\n"           -> default by CGI */
        /* "Cache-Control: no-store\r\n"  -> should be set by the httpd */
        "Content-Type: text/plain; charset=utf-8\r\n"
        "\r\n"
        "Local time: %s\r\n"
        "Remote address: %s\r\n",
        timebuf, remote_addr);
}
