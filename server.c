#include "functions.h"

// TODO МБ сделать пересылку файлов

int main() {
    int client_sock;
    struct sockaddr_in server_address, client_address;
    socklen_t address_size;
    char buffer[MESSAGE_SIZE];
    int n;

    int server_sock = create_socket();

    memset(&server_address, '\0', sizeof(server_address));
    configure_address(&server_address, PORT, LOCALHOST);

    n = bind(server_sock, (struct sockaddr *)&server_address, sizeof(server_address));
    if (n < 0) {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %i\n", PORT);

    if (listen(server_sock, 5) < 0) {
        perror("[-]Listening error");
        exit(1);
    }
    printf("Listening...\n");

    while (1) {

        // Ожидание подключения клиента
        address_size = sizeof(client_address);
        client_sock = accept(server_sock, (struct sockaddr *)&client_address, &address_size);
        if (client_sock < 0) {
            printf("[-]Accepting error");
            exit(1);
        }
        printf("[+]Client connected.\n");

        // Прием ответа клиента
        while (recv(client_sock, buffer, sizeof(buffer), 0) != -1) {

            printf("%sClient:%s %s\n", COLOR_RED, COLOR_NO, buffer);

            if (strcmp(buffer, LS_COMMAND_SERVER) == 0) {
                print_dir("/");
            }

            if (strcmp(buffer, PWD_COMMAND_SERVER) == 0) {
                print_full_path();
            }

            // Закрытие сокета
            if (strcmp(buffer, EXIT_COMMAND_CLIENT) == 0) {
                close_socket(client_sock);
                printf("[+]Client disconnected.\n");
            }

            // Очистка буфера
            bzero(buffer, MESSAGE_SIZE);
            rewind(stdin);
        }
    }
}
