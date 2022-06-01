//#include "functions.h"
//
//bool create_new_thread = false;
//bool thread_ended      = false;
//
//int current_clients = 0;
//
//int server_sock;
//int client_sockets[MAX_CLIENTS];
//pthread_t client_threads[MAX_CLIENTS];
//
//sem_t sem;
//
//void print_current_sem_value(int index) {
//    int value;
//    sem_getvalue(&sem, &value);
//    printf("Client connections = %d / %d\n", value, MAX_CLIENTS);
//    printf("Client max index: %d\n", index);
//    printf("Current clients: %d\n", current_clients);
//    puts("");
//}
//
//void cancel_client_threads(int amount) {
//    for (int i = 0; i < amount; i++) {
//        pthread_cancel(client_threads[i]);
//        send_by_fd(client_sockets[i], "EXIT!!!");
//    }
//}
//
//void *handle_server(void* arg) {
//    int index = *((int*)arg) - 1;
//    char buffer[MESSAGE_SIZE];
//
//    struct sockaddr_in client_address;
//
//    socklen_t address_size = sizeof(client_address);
//    client_sockets[index] = check(accept(
//                                          server_sock,
//                                          (struct sockaddr *)&client_address,
//                                          &address_size),
//                                  "Accept error");
//    printf("Client connected.\n");
//
//    create_new_thread = true;
//
//    puts("In handle_server():...");
//    print_current_sem_value(index);
//
//    while (recv(client_sockets[index], buffer, sizeof(buffer), 0) != -1) {
//
//        printf("%sClient:%s %s", COLOR_RED, COLOR_NO, buffer);
//
//        if (strcmp(buffer, LS_COMMAND_SERVER) == 0) {
//            print_dir();
//        }
//
//        if (strcmp(buffer, PWD_COMMAND_SERVER) == 0) {
//            print_full_path();
//        }
//
//        if (strcmp(buffer, FILE_INFO) == 0) {
//            get_file_info();
//        }
//
//        if (strcmp(buffer, EXIT_COMMAND_CLIENT) == 0) {
//            thread_ended = true;
//            close_socket(client_sockets[index]);
//            sem_post(&sem);
//            printf("Client %d disconnected!\n", index);
//            print_current_sem_value(index);
//            return NULL;
//        }
//
//        if (strcmp(buffer, EXIT_COMMAND_ALL) == 0) {
////            index--;
//            close_socket(server_sock);
//            close_socket(client_sockets[index]);
//            cancel_client_threads(index);
//            printf("Server closed.\n");
//            thread_ended = true;
//            exit(0);
//        }
//
//        bzero(buffer, MESSAGE_SIZE);
//        rewind(stdin);
//    }
////    thread_ended = true;
////    sem_post(&sem);
//}
//
//int main(int argc, char **argv) {
//
//    sem_init(&sem, 0, MAX_CLIENTS);
//
//    char *ip;
//
//    if (argc < 2) {
//        printf("No ip provided! Using localhost: %s\n", LOCALHOST);
//        ip = LOCALHOST;
//    } else {
//        ip = argv[1];
//    }
//
//    server_sock = create_socket();
//    struct sockaddr_in server_address;
//
//    memset(&server_address, '\0', sizeof(server_address));
//
//    configure_address(&server_address, PORT, ip);
//
//    check(bind(server_sock,
//               (struct sockaddr *)&server_address,
//               sizeof(server_address)),
//          "Bind error");
//    printf("Bind to the port number: %i\n", PORT);
//
//    check(listen(server_sock, 5),
//          "Listening error");
//    printf("Listening...\n");
//
//    int index = 0;
//
//    puts("Initialization:");
//    print_current_sem_value(index);
//
//    pthread_create(&client_threads[index++],
//                   NULL,
//                   handle_server,
//                   (void *) &(index));
//    current_clients++;
//
//    while (true) {
//        if (create_new_thread && current_clients <= MAX_CLIENTS) {
//            sem_wait(&sem);
//            puts("In create_new_thread block:");
//            print_current_sem_value(index);
//
//            pthread_create(&client_threads[index++],
//                           NULL,
//                           handle_server,
//                           (void *) &(index));
//            current_clients++;
//
//            create_new_thread = false;
//        }
//        if (thread_ended) {
//            pthread_join(client_threads[--index], NULL);
//            current_clients--;
//            puts("Thread joined");
//            thread_ended = false;
//            puts("If thread_ended:");
//            print_current_sem_value(index);
//        }
//        sleep(1);
//    }
//}


#include "functions.h"

bool create_new_thread = false;
bool thread_ended      = false;

int server_sock;
int client_sockets[MAX_CLIENTS];
pthread_t client_threads[MAX_CLIENTS];


void cancel_server_threads(int amount) {
    for (int i = 0; i < amount; i++) {
        pthread_cancel(client_threads[i]);
    }
}

void *handle_server(void* arg) {
    int index = *((int*)arg) - 1;
    char buffer[MESSAGE_SIZE];

    struct sockaddr_in client_address;

    socklen_t address_size = sizeof(client_address);
    client_sockets[index] = check(accept(
                                          server_sock,
                                          (struct sockaddr *)&client_address,
                                          &address_size),
                                  "Accept error");
    printf("Client connected.\n");

    create_new_thread = true;

    while (recv(client_sockets[index], buffer, sizeof(buffer), 0) != -1) {

        printf("%sClient:%s %s", COLOR_RED, COLOR_NO, buffer);

        if (strcmp(buffer, LS_COMMAND_SERVER) == 0) {
            print_dir();
        }

        if (strcmp(buffer, PWD_COMMAND_SERVER) == 0) {
            print_full_path();
        }

        if (strcmp(buffer, FILE_INFO) == 0) {
            get_file_info();
        }

        if (strcmp(buffer, EXIT_COMMAND_CLIENT) == 0) {
            close_socket(client_sockets[index]);
            printf("Client disconnected.\n");
        }

        if (strcmp(buffer, EXIT_COMMAND_ALL) == 0) {
            close_socket(server_sock);
            close_socket(client_sockets[index]);
            cancel_server_threads(index);
            printf("Server closed.\n");
            exit(0);
        }

        bzero(buffer, MESSAGE_SIZE);
        rewind(stdin);
    }
    thread_ended = true;
    return NULL;
}

int main(int argc, char **argv) {

    char *ip;

    if (argc < 2) {
        printf("No ip provided! Using localhost: %s\n", LOCALHOST);
        ip = LOCALHOST;
    } else {
        ip = argv[1];
    }

    server_sock = create_socket();
    struct sockaddr_in server_address;

    memset(&server_address, '\0', sizeof(server_address));

    configure_address(&server_address, PORT, ip);

    int yes = 1;
    check(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)),
          "Setsockopt error");

    check(bind(server_sock,
               (struct sockaddr *)&server_address,
               sizeof(server_address)),
          "Bind error");
    printf("Bind to the port number: %i\n", PORT);

    check(listen(server_sock, 5),
          "Listening error");
    printf("Listening...\n");

    int index = 0;

    pthread_create(&client_threads[index++],
                   NULL,
                   handle_server,
                   (void *)&(index));

    while (true) {
        if (create_new_thread) {
            puts("Client created!");
            create_new_thread = false;
            pthread_create(&client_threads[index++],
                           NULL,
                           handle_server,
                           (void *) &(index));
        }
        if (thread_ended) {
            pthread_join(client_threads[--index], NULL);
            puts("Thread joined");
            thread_ended = false;
        }
        sleep(1);
    }
}