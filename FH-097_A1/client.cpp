#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8091

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[4096];
    int bytes_received;

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);


    // For connecting to the server on a different machine, specify the server's IP address
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    //printf("Connected to server. Type 'exit' to quit.\n");

    while (1) {

        // receive server's jokes
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';

            // Split the received message to extract repeatMsg
            char* repeatMsgPtr = strstr(buffer, "\nServer:");
            
            /* this condition is for showing the client last part of the joke and
               if there is no joke to tell by the server thus the client terminates itself 
               after the last msg from server,
               also if it wants to get any more jokes or not */ 
            if (repeatMsgPtr != NULL) {
                *repeatMsgPtr = '\0';  // Replace '\n' with null terminator
                printf("Server: %s\n", buffer);  // Print the part before repeatMsg

                /* Check if repeatMsgPtr is "I have no more jokes to tell." 
                    -> this is for last joke buffer has last joke */
                if (strcmp(repeatMsgPtr + strlen("\nServer:"), " I have no more jokes to tell.") == 0) {
                    printf("Server:%s\n", repeatMsgPtr + strlen("\nServer:"));  // Print repeatMsg
                    
                    //client terminates itself
                    close(client_socket);
                    return 0;

                }

                /* for more jokes msgs (options (Y/N)) */
                else {
                    printf("Server:%s\n", repeatMsgPtr + strlen("\nServer:"));  // Print repeatMsg
                }
            }
            /* for server joke msgs */
            else {
                printf("Server: %s\n", buffer);
            }
        }

        // clients says the following parts of the joke as input from client
        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        if (strcmp(buffer, "N") == 0) {
            // Client terminates itself
            close(client_socket);
            return 0;
        }

        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);

    return 0;
}
