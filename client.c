#include "functions.h"

int main(int argc, char **argv) {

    char *ip;

    if (argc < 2) {
        printf("No ip provided! Using localhost: %s\n", LOCALHOST);
        ip = LOCALHOST;
    } else {
        ip = argv[1];
    }

    signal(SIGPIPE, pipe_handler);

    struct sockaddr_in address;
    char buffer[MESSAGE_SIZE];
    int sock = create_socket();

    memset(&address, '\0', sizeof(address));
    configure_address(&address, PORT, ip);

    check(connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0,
          "Connection error\n");
    printf("Connection opened.\n");

    const char *test_message = "Test message.\n";
    send(sock, test_message, strlen(test_message), 0);

    print_help();

    while (true) {

        bzero(buffer, MESSAGE_SIZE);
        printf("Enter command: ");

        get_string(buffer, MESSAGE_SIZE);
        printf("Client: %s\n", buffer);
        send_by_fd(sock, buffer);

        if (strcmp(buffer, HELP_COMMAND) == 0) {
            print_help();
        } else if (strcmp(buffer, LS_COMMAND_CLIENT) == 0) {
            print_dir();
        } else if (strcmp(buffer, PWD_COMMAND_CLIENT) == 0) {
            print_full_path();
        } else if (strcmp(buffer, EXIT_COMMAND_CLIENT) == 0 || strcmp(buffer, EXIT_COMMAND_ALL) == 0) {
            close_socket(sock);
            printf("Disconnected from the server.\n");
            exit(0);
        } else if (
                strcmp(buffer, LS_COMMAND_SERVER) != 0 &&
                strcmp(buffer, PWD_COMMAND_SERVER) != 0 &&
                strcmp(buffer, FILE_INFO) != 0 &&
                strcmp(buffer, EXIT_COMMAND_ALL) != 0) {
            puts("Unknown command!");
        }

    }
}