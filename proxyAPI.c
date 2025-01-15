#include "proxyAPI.h"
#include "simpleSocketAPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void relayData(int clientSock, int serverSock) {
  char buffer[BUFFER_SIZE];
  ssize_t bytes;

  while ((bytes = read(clientSock, buffer, sizeof(buffer))) > 0) {
    write(serverSock, buffer, bytes);
  }
}

void handlePortCommand(int clientSock, int serverSock, char *buffer) {
  // Remplacez la commande PORT par PASV
  snprintf(buffer, BUFFER_SIZE, "PASV\r\n");
  write(serverSock, buffer, strlen(buffer));

  // Lire la réponse PASV
  ssize_t bytes = read(serverSock, buffer, BUFFER_SIZE - 1);
  if (bytes > 0) {
    buffer[bytes] = '\0';
    printf("Réponse PASV : %s", buffer);

    // Parsez l'adresse IP et le port depuis la réponse PASV
    char *start = strchr(buffer, '(');
    char *end = strchr(buffer, ')');
    if (start && end && end > start) {
      char pasvInfo[BUFFER_SIZE];
      strncpy(pasvInfo, start + 1, end - start - 1);
      pasvInfo[end - start - 1] = '\0';

      int ip[4], p1, p2;
      sscanf(pasvInfo, "%d,%d,%d,%d,%d,%d", &ip[0], &ip[1], &ip[2], &ip[3], &p1,
             &p2);
      int dataPort = p1 * 256 + p2;

      // Construire l'adresse IP du serveur
      char serverIP[BUFFER_SIZE];
      snprintf(serverIP, sizeof(serverIP), "%d.%d.%d.%d", ip[0], ip[1], ip[2],
               ip[3]);

      // Établir la connexion de données avec le serveur
      int dataSock;
      if (connect2Server(serverIP, dataPort, &dataSock) == 0) {
        printf("Connecté au canal de données : %s:%d\n", serverIP, dataPort);

        // Relayer les données entre le client et le serveur ici
        relayData(clientSock, dataSock);
        close(dataSock);
      } else {
        perror("Erreur connexion canal de données");
      }
    }
  }
}

void handleFTP(int clientSock) {
  char buffer[BUFFER_SIZE];
  int serverSock;
  ssize_t bytes;

  // Recevoir la commande USER du client
  bytes = read(clientSock, buffer, BUFFER_SIZE - 1);
  if (bytes <= 0) {
    perror("Erreur lecture client");
    close(clientSock);
    exit(EXIT_FAILURE);
  }
  buffer[bytes] = '\0';
  printf("Commande reçue : %s", buffer);

  // Vérifiez si c'est une commande USER
  if (strncmp(buffer, "USER ", 5) == 0) {
    char *userPart = buffer + 5;
    char *sep = strchr(userPart, '@');
    if (sep) {
      *sep = '\0';
      char *username = userPart;
      char *servername = sep + 1;

      // Connectez-vous au serveur FTP
      if (connect2Server(servername, "21", &serverSock) == 0) {
        printf("Connecté au serveur FTP : %s\n", servername);

        // Relayer la commande USER vers le serveur
        snprintf(buffer, BUFFER_SIZE, "USER %s\r\n", username);
        write(serverSock, buffer, strlen(buffer));

        // Relayez toutes les commandes/réponses
        while ((bytes = read(clientSock, buffer, BUFFER_SIZE - 1)) > 0) {
          buffer[bytes] = '\0';
          printf("Commande client : %s", buffer);

          if (strncmp(buffer, "PORT ", 5) == 0) {
            handlePortCommand(clientSock, serverSock, buffer);
          } else if (strncmp(buffer, "PASS ", 5) == 0) {
            // Relayer la commande PASS
            write(serverSock, buffer, bytes);
            // Lire la réponse du serveur
            bytes = read(serverSock, buffer, BUFFER_SIZE - 1);
            if (bytes <= 0)
              break;
            buffer[bytes] = '\0';
            printf("Réponse serveur : %s", buffer);
            write(clientSock, buffer, bytes);
          } else {
            // Relayer toutes les autres commandes
            write(serverSock, buffer, bytes);

            // Lire la réponse du serveur
            bytes = read(serverSock, buffer, BUFFER_SIZE - 1);
            if (bytes <= 0)
              break;
            buffer[bytes] = '\0';
            printf("Réponse serveur : %s", buffer);
            write(clientSock, buffer, bytes);
          }
        }
      }
      close(serverSock);
    }
  }

  close(clientSock);
  exit(EXIT_SUCCESS); // Le processus enfant se termine après la session
}