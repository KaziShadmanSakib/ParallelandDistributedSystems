#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h> 
#include<bits/stdc++.h>
#include <string>
#include <time.h>

#define PORT 8091
#define MAX_CLIENTS 3
#define TIMEOUT_SECONDS 10

int num_clients = 0;
int server_socket;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


std::vector<std::string> firstParts;
std::vector<std::string> lastParts;

/* jokesDB.txt is the database used for the server to tell jokes.
   This function loads the jokesDB.txt file for the server. */
void loadJokesDatabase() {
    std::ifstream jokesFile("jokesDB.txt");
    std::string line;

    /* Extracts jokes to tell by the server using the database format,
       The first part of the joke is extracted and kept in firstParts,
       The last part of the joke is extracted and kept in lastParts. */
    while (std::getline(jokesFile, line)) {
        size_t pos = line.find(" : ");
        if (pos != std::string::npos) {
            firstParts.push_back(line.substr(pos + 3, line.find(". ", pos + 3) - pos - 3));
            lastParts.push_back(line.substr(line.find(". ", pos + 3) + 2));
        }
    }
    jokesFile.close();
}

/* Each client thread follows this handler separately (Parallel and Synchronization) */
void* handle_client(void* client_socket_ptr) {
    int client_socket = *((int*)client_socket_ptr);
    char buffer[4096];
    int bytes_received;

    srand(time(NULL)); // Initialize random seed

    std::vector<int> usedIndices; // Store indices of used jokes

    /* The server starts to send jokes until all the jokes are finished telling (15) */
    while (usedIndices.size() < firstParts.size()) {
        // server initiates after the connection by saying Knock knock
        const char* startJoke = "Knock knock";
        send(client_socket, startJoke, strlen(startJoke), 0);

        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0)
            break;

        buffer[bytes_received] = '\0';
        printf("Client %d: %s\n", client_socket, buffer);

        
        int numFirstParts = firstParts.size();

        /* this is a do while loop used to check if the same joke has been */
        int randomIndex;
        do {
            randomIndex = rand() % numFirstParts;
        } while (std::find(usedIndices.begin(), usedIndices.end(), randomIndex) != usedIndices.end());

        usedIndices.push_back(randomIndex);

        /* Did the client said Who's there? correctly or not */
        if (strcasecmp(buffer, "Who's there?") == 0) {
            send(client_socket, firstParts[randomIndex].c_str(), firstParts[randomIndex].length(), 0);

            bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                printf("Client %d: %s\n", client_socket, buffer);

                std::string receivedMsg = firstParts[randomIndex] + " who?";

                /* Did the client said first part of joke + who? correctly or not */
                if (strcasecmp(buffer, receivedMsg.c_str()) == 0) {
                    
                    /* Last joke has come */
                    if(usedIndices.size() == firstParts.size()){
                        const char* noMoreJokesMsg = "\nServer: I have no more jokes to tell.";
                        std::string lastPartJoke = lastParts[randomIndex] + noMoreJokesMsg;
                        send(client_socket, lastPartJoke.c_str(), lastPartJoke.length(), 0);
                    }

                    /* Choice given to client if it wants more jokes or not */
                    else{
                        const char* repeatMsg = "\nServer: Would you like to listen to another? (Y/N)";
                        std::string lastPartJoke = lastParts[randomIndex] + repeatMsg;
                        send(client_socket, lastPartJoke.c_str(), lastPartJoke.length(), 0);
                    }
                    
                    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
                    if (bytes_received > 0) {
                        buffer[bytes_received] = '\0';
                        if (strcasecmp(buffer, "Y") == 0) {
                            continue; // Continue to the next iteration to send another joke
                        } else if (strcasecmp(buffer, "N") == 0) {
                            break; // Exit the loop if the client responds with "N"
                        }
                    }
                }
                /* if the format of the last joke was incorrectly sent by the client */
                else {
                    const char* errorMsg = "You are supposed to say, \"";
                    const char* tryAgainMsg = "\". Let’s try again.\nServer: ";
                    std::string combinedMsg = errorMsg + receivedMsg + tryAgainMsg;

                    send(client_socket, combinedMsg.c_str(), combinedMsg.length(), 0);
                }
            }
        } 

        /* If the first part of the joke was inccorectly sent by the client */
        else {
            const char* errorMsg = "You are supposed to say, \"Who’s there?\". Let’s try again.\nServer: ";
            send(client_socket, errorMsg, strlen(errorMsg), 0);
        }
    }

    close(client_socket);

    // critical region for synchronization
    pthread_mutex_lock(&mutex);
    num_clients--;
    pthread_mutex_unlock(&mutex);

    return NULL;
}


/* this function checks if there are any clients using the server or not,
   if there are no new clients joining during 10s time then the server will auto terminate */
void* termination_check(void*) {
    struct timespec ts;
    struct timeval now;
    struct timespec timeout;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (num_clients == 0) {
            gettimeofday(&now, NULL);
            timeout.tv_sec = now.tv_sec + TIMEOUT_SECONDS;
            timeout.tv_nsec = now.tv_usec * 1000;

            ts = timeout;

            int rc = pthread_cond_timedwait(&cond, &mutex, &ts);

            if (rc == ETIMEDOUT && num_clients == 0) {
                printf("No clients for %d seconds. Server terminating...\n", TIMEOUT_SECONDS);
                pthread_mutex_unlock(&mutex);
                close(server_socket);
                exit(EXIT_SUCCESS);
            }
        }

        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Load jokes database
    loadJokesDatabase();

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Bind the socket
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind the socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen on the socket
    if (listen(server_socket, 3) < 0) {
        perror("Failed to listen on the socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server started. Listening on port %d\n", PORT);

    // Create termination check thread
    pthread_t termination_thread;
    if (pthread_create(&termination_thread, NULL, termination_check, NULL) != 0) {
        perror("Failed to create termination thread");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    while (num_clients < MAX_CLIENTS) {
        // Accept a new connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Failed to accept client connection");
            continue;
        }

        // critical region for synchronization
        pthread_t client_thread;
        pthread_mutex_lock(&mutex);
        num_clients++;
        pthread_mutex_unlock(&mutex);

        if (pthread_create(&client_thread, NULL, handle_client, (void*)&client_socket) < 0) {
            perror("Failed to create client thread");
            close(client_socket);
            continue;
        }

        printf("New client connected. Client count: %d\n", num_clients);

        pthread_detach(client_thread);
    }

    printf("All clients served. Server terminating...\n");

    // Signal termination thread and wait for it
    pthread_cond_signal(&cond);
    pthread_join(termination_thread, NULL);

    close(server_socket);

    return 0;
}
