#include "functions.h"

void get_string(char* string, int len) {
    fgets(string, len, stdin);

    if (strlen(string) == len - 1)
        while ((getchar()) != '\n');
}

void print_help() {
    printf("Commands:\n"
           "help - print list of commands\n"
           "ls   - print files in client dir\n"
           "!ls  - print files in server dir\n"
           "pwd  - print full client path\n"
           "!pwd - print full server path\n"
//           "send - send file\n"
//           "get  - get file\n"
           "exit - close client\n\n");
}

// simple dir_walk implementation
void print_dir(char *path) {

    if ( strcmp(path, "") == 0 ) {
        path = malloc(FILENAME_SIZE);

        if (path == NULL) {
            printf("malloc error!");
            return;
        }

        path = getcwd(path, FILENAME_SIZE);
        printf("Empty path provided! Showing info in '%s':\n", path);
    }

    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);
    if (dir == NULL) {
        printf("%sprint_dir error!%s Wrong path: %s\n", COLOR_RED, COLOR_NO, path);
        return;
    }

    printf("\n%sDirectory contains:%s\n", COLOR_RED, COLOR_NO);
    while ( (entry = readdir(dir)) ) {
        char *dirName = entry->d_name;

        if (entry->d_type == DT_DIR && (strcmp(dirName, ".") == 0 || strcmp(dirName, "..") == 0)) {
            continue;
        }

        printf("%s\n", entry->d_name);
    }
}

void print_full_path() {
    char *path = malloc(FILENAME_SIZE);
    path = getcwd(path, FILENAME_SIZE);
    printf("Path: %s\n", path);
}

void close_socket(int socket_fd) {
    if (close(socket_fd)) {
        fputs("[-]Error closing socket", stderr);
        exit(EXIT_FAILURE);
    }
}

void configure_address(struct sockaddr_in *address, int port, const char *ip) {
    address->sin_family = AF_INET;
    address->sin_port = port;
    address->sin_addr.s_addr = inet_addr(ip);
}

int create_socket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");
    return sock;
}

void send_to_server(int sock, char *buffer) {
    bzero(buffer, MESSAGE_SIZE);
    printf("Enter command: ");
    get_string(buffer, MESSAGE_SIZE);
    printf("Client: %s\n", buffer);
    if (send(sock, buffer, strlen(buffer), 0) < 0) {
        puts("Error while sending!");
        exit(1);
    }
}
