#include "functions.h"

int main() {

    signal(SIGPIPE, pipe_handler);

    struct sockaddr_in address;
    char buffer[MESSAGE_SIZE];
    int sock = create_socket();

    memset(&address, '\0', sizeof(address));
    configure_address(&address, PORT, LOCALHOST);

    check(connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0,
          "[-]Connection error\n");
    printf("Connected to the server.\n");


    while (1) {
        send_to_server(sock, buffer);

        if (strcmp(buffer, HELP_COMMAND) == 0) {
            print_help();
        } else if (strcmp(buffer, LS_COMMAND_CLIENT) == 0) {
            print_dir("");
        } else if (strcmp(buffer, PWD_COMMAND_CLIENT) == 0) {
            print_full_path();
        } else if (strcmp(buffer, "send\n") == 0) {
//            send_file_menu(sock);
        } else if (strcmp(buffer, EXIT_COMMAND_CLIENT) == 0) {
            close_socket(sock);
            printf("Disconnected from the server.\n");
            exit(0);
        } else if (strcmp(buffer, "!exit\n") == 0) {
            close_socket(sock);
            printf("[+]Client closed.\n");
            exit(0);
        } else if (
                strcmp(buffer, LS_COMMAND_SERVER) != 0 &&
                strcmp(buffer, PWD_COMMAND_SERVER) != 0 &&
                strcmp(buffer, EXIT_COMMAND_ALL) != 0) {
            puts("Unknown command!");
        }

    }
}
