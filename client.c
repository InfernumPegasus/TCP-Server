#include "functions.h"

int main(int argc, char **argv) {
    struct sockaddr_in address;
    char buffer[MESSAGE_SIZE];
    int sock = create_socket();

    memset(&address, '\0', sizeof(address));
    configure_address(&address, PORT, LOCALHOST);

    if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        printf("[-]Connection error");
        exit(1);
    }
    printf("Connected to the server.\n");

    while (1) {
        send_to_server(sock, buffer);

//        if (is_command_correct(buffer) != 0) {
//            printf("Unknown command!\n");
//            continue;
//        }

        if (strcmp(buffer, HELP_COMMAND) == 0) {
            print_help();
        }
        if (strcmp(buffer, LS_COMMAND_CLIENT) == 0) {
            print_dir("");
        }
        if (strcmp(buffer, PWD_COMMAND_CLIENT) == 0) {
            print_full_path();
        }
        if (strcmp(buffer, "send\n") == 0) {
//            send_file_menu(sock);
        }
        if (strcmp(buffer, EXIT_COMMAND_CLIENT) == 0) {
            // ЗАКРЫТИЕ СОКЕТА
            close(sock);
            printf("Disconnected from the server.\n");
            return 0;
        }
    }

//    // ОТПРАВКА СООБЩЕНИЯ СЕРВЕРУ
//    bzero(buffer, MESSAGE_SIZE);
//    printf("Enter command: ");
//    get_string(buffer, MESSAGE_SIZE);
//    printf("Client: %s\n", buffer);
//    send(sock, buffer, strlen(buffer), 0);
//
//
//    // ПОЛУЧЕНИЕ ОТВЕТА ОТ СЕРВЕРА
//    bzero(buffer, MESSAGE_SIZE);
//    recv(sock, buffer, sizeof(buffer), 0);
//    printf("Server: %s\n", buffer);
}
