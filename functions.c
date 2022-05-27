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
           "file - get file info\n"
           "exit - close client\n\n");
}

void print_dir() {
    char *path = malloc(sizeof(char) * PATH_MAX);
    printf("Enter path: ");
    get_string(path, PATH_MAX);
    path[strcspn(path, "\n")] = 0;

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
    if (path == NULL) {
        perror("Print path error");
        return;
    }
    printf("Path: %s\n", path);
}

void close_socket(int socket_fd) {
    check(close(socket_fd),
          "Error closing socket");
}

void configure_address(struct sockaddr_in *address, int port, const char *ip) {
    address->sin_family = AF_INET;
    address->sin_port = port;
    address->sin_addr.s_addr = inet_addr(ip);
}

int create_socket() {
    int sock = check(socket(AF_INET, SOCK_STREAM, 0),
                     "Socket error");

    printf("Socket with value %d created.\n", sock);
    return sock;
}

void send_by_fd(int sock, char *buffer) {
    check(send(sock, buffer, strlen(buffer), 0),
          "Error while sending!");
}

int check(ssize_t exp, const char *msg) {
    if (exp == SOCKET_ERROR) {
        perror(msg);
        exit(1);
    }
    return (int)exp;
}

void pipe_handler(int) {
    fputs("Wrong connection! Try again with another ip.\n", stderr);
    exit(1);
}

void get_file_info() {
    char filename[FILENAME_MAX];

    printf("Enter filename: ");
    get_string(filename, FILENAME_MAX);
    filename[strcspn(filename, "\n")] = 0;

    struct stat file_stats;

    if (stat(filename, &file_stats) != 0) {
        perror("Getting stats error!");
        return;
    }

    printf("Inode number: %lu\n",          file_stats.st_ino);
    printf("User ID of owner: %u\n",       file_stats.st_uid);
    printf("Group ID of owner: %u\n",      file_stats.st_gid);
    printf("Total file size: %lu bytes\n", file_stats.st_size);
    printf("Last status change: %s",       ctime(&file_stats.st_ctime));
    printf("Last file access: %s",         ctime(&file_stats.st_atime));
    printf("Last file modification: %s",   ctime(&file_stats.st_mtime));
}