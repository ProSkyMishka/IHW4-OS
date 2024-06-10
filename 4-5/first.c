#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

void work(int client_socket, int working_time, struct FieldSize field_size) {
    struct Task task;
    task.gardener_id = 1;
    task.working_time = working_time;
    int rows = field_size.rows;
    int columns = field_size.columns;

    int i = 0;
    int j = 0;
    task.status = 0;
    while (i < rows) {
        while (j < columns) {
            task.plot_i = i;
            task.plot_j = j;
            sendHandleRequest(client_socket, task);
            ++j;
        }

        ++i;
        --j;

        while (j >= 0) {
            task.plot_i = i;
            task.plot_j = j;
            sendHandleRequest(client_socket, task);
            --j;
        }

        ++i;
        ++j;
    }

    task.status = 1;
    sendHandleRequest(client_socket, task);
}

int main(int argc, char *argv[]) {
    int client_socket;
    unsigned short server_port;
    int working_time;
    char *server_ip;
    char buffer[BUFFER_SIZE];
    int bytes_received, total_bytes_received;

    if (argc != 4) {
        fprintf(stderr, "3 args: %s Server address and port, Working time\n", argv[0]);
        exit(1);
    }

    server_ip = argv[1];

    server_port = atoi(argv[2]);
    working_time = atoi(argv[3]);

    client_socket = createClientSocket(server_ip, server_port);

    struct pollfd fd;
    fd.fd = client_socket;
    fd.events = POLLIN;

    struct Task task;
    task.status = 2;

    if (trySend(client_socket, &task, sizeof(task), 0) < 0) {
        printf("Server error\n");
        exit(0);
    }

    struct FieldSize field_size;
    if ((bytes_received = recv(client_socket, &field_size, sizeof(field_size), 0)) !=
        sizeof(field_size)) {
        perror("bad recv()");
        exit(-1);
    }

    work(client_socket, working_time, field_size);
    printf("Gardener 1 finish work\n");

    close(client_socket);
    exit(0);
}
