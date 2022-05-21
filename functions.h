#ifndef SERVER_FUNCTIONS_H
#define SERVER_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>

#define MAX_CLIENTS 4

#define SOCKET_ERROR -1

/* Buffers */
#define MESSAGE_SIZE  256
#define FILENAME_SIZE 256

/* Connection stuff */
#define PORT 5566
#define LOCALHOST "127.0.0.1"

/* Console colors */
#define COLOR_RED "\033[1;31;40m"
#define COLOR_NO  "\033[1;0m"


/* Commands */

// ls command
#define LS_COMMAND_CLIENT "ls\n"
#define LS_COMMAND_SERVER "!ls\n"

// pwd command
#define PWD_COMMAND_CLIENT "pwd\n"
#define PWD_COMMAND_SERVER "!pwd\n"

// help command
#define HELP_COMMAND "help\n"

// exit command
#define EXIT_COMMAND_CLIENT "exit\n"
#define EXIT_COMMAND_ALL "!exit\n"


void get_string(char* string, int len);

void print_help();

void print_full_path();

void print_dir(char *path);

void close_socket(int socket_fd);

void configure_address(struct sockaddr_in *address, int port, const char *ip);

int create_socket();

void send_to_server(int sock, char *buffer);

int check(ssize_t exp, const char *msg);

void pipe_handler(int);

// TODO
//void send_file();
//void receive_file();

// TODO функция для считывания порта для создания и подключения к серверу
//char *parse_port(char *arg);

#endif //SERVER_FUNCTIONS_H
