#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>      // Pour errno
#include <sys/time.h>   // Pour struct timeval
#include "./simpleSocketAPI.h"

#define SERVADDR "127.0.0.1"
#define SERVPORT "0"
#define LISTENLEN 1
#define MAXBUFFERLEN 1024
#define MAXHOSTLEN 64
#define MAXPORTLEN 64

/*
 * Gère la connexion et les échanges avec un client FTP (descSockCOM)
 * puis relaye vers le serveur FTP (sockServerCMD).
 */
void fils(int descSockCOM);

int main()
{
    int ecode;
    char serverAddr[MAXHOSTLEN];
    char serverPort[MAXPORTLEN];
    int descSockRDV;
    int descSockCOM;
    struct addrinfo hints;
    struct addrinfo *res;
    struct sockaddr_storage myinfo;
    struct sockaddr_storage from;
    socklen_t len;

    // 1) Socket de rendez-vous
    descSockRDV = socket(AF_INET, SOCK_STREAM, 0);
    if (descSockRDV == -1)
    {
        perror("Erreur création socket RDV");
        exit(EXIT_FAILURE);
    }

    // 2) Préparation de hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags    = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family   = AF_INET;

    // 3) getaddrinfo pour bind
    ecode = getaddrinfo(SERVADDR, SERVPORT, &hints, &res);
    if (ecode != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode));
        close(descSockRDV);
        exit(EXIT_FAILURE);
    }

    // 4) bind()
    ecode = bind(descSockRDV, res->ai_addr, res->ai_addrlen);
    if (ecode == -1)
    {
        perror("Erreur liaison de la socket de RDV");
        freeaddrinfo(res);
        close(descSockRDV);
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(res);

    // 5) Récupération du port effectif si SERVPORT=0
    len = sizeof(myinfo);
    ecode = getsockname(descSockRDV, (struct sockaddr *)&myinfo, &len);
    if (ecode == -1)
    {
        perror("SERVEUR: getsockname");
        close(descSockRDV);
        exit(EXIT_FAILURE);
    }
    ecode = getnameinfo((struct sockaddr *)&myinfo, sizeof(myinfo),
                        serverAddr, MAXHOSTLEN,
                        serverPort, MAXPORTLEN,
                        NI_NUMERICHOST | NI_NUMERICSERV);
    if (ecode != 0)
    {
        fprintf(stderr, "error in getnameinfo: %s\n", gai_strerror(ecode));
        close(descSockRDV);
        exit(EXIT_FAILURE);
    }
    printf("L'adresse d'ecoute est: %s\n", serverAddr);
    printf("Le port d'ecoute est: %s\n", serverPort);

    // 6) listen
    ecode = listen(descSockRDV, LISTENLEN);
    if (ecode == -1)
    {
        perror("Erreur listen()");
        close(descSockRDV);
        exit(EXIT_FAILURE);
    }

    // 7) Boucle d'acceptation
    while (true)
    {
        len = sizeof(from);
        descSockCOM = accept(descSockRDV, (struct sockaddr *)&from, &len);
        if (descSockCOM == -1)
        {
            perror("Erreur accept()");
            continue;
        }

        // Gère le client
        fils(descSockCOM);
        // (descSockCOM est fermé dans fils())
    }

    // Jamais atteint si boucle infinie
    close(descSockRDV);
    return 0;
}

void fils(int descSockCOM)
{
    char buffer[MAXBUFFERLEN];
    int ecode;

    // 1) Envoi d'un message de bienvenue
    strcpy(buffer, "220 Bienvenue au proxy :) \n");
    if (write(descSockCOM, buffer, strlen(buffer)) == -1)
    {
        perror("Ecriture bienvenue");
        goto close_client;
    }

    // 2) Lecture de "USER login@server"
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture USER login@server");
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("Recu du client ftp: %s\n", buffer);

    // Extraction login et serverName
    char login[50], serverName[50];
    if (sscanf(buffer, "%50[^@]@%50s", login, serverName) != 2)
    {
        fprintf(stderr, "Format de login@server incorrect.\n");
        goto close_client;
    }
    strncat(login, "\r\n", sizeof(login) - strlen(login) - 1);

    // 3) Connexion au serveur FTP distant (port 21)
    int sockServerCMD;
    ecode = connect2Server(serverName, "21", &sockServerCMD);
    if (ecode == -1)
    {
        perror("Connexion serveur FTP");
        goto close_client;
    }

    // 4) Lecture de la bannière 220
    ecode = read(sockServerCMD, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture banniere 220 serveur");
        close(sockServerCMD);
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("Recu du serveur: %s\n", buffer);

    // 5) Envoi USER login au serveur
    if (write(sockServerCMD, login, strlen(login)) == -1)
    {
        perror("Ecriture login vers serveur");
        close(sockServerCMD);
        goto close_client;
    }

    // 6) Lecture reponse 331 (demande PASS)
    ecode = read(sockServerCMD, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture reponse 331");
        close(sockServerCMD);
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("Recu du serveur: %s\n", buffer);

    // 7) Renvoi 331 au client
    if (write(descSockCOM, buffer, strlen(buffer)) == -1)
    {
        perror("Renvoi 331 au client");
        close(sockServerCMD);
        goto close_client;
    }

    // 8) Lecture PASS depuis le client
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture PASS client");
        close(sockServerCMD);
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("Recu du client ftp (PASS): %s\n", buffer);

    // 9) Envoi PASS au serveur
    if (write(sockServerCMD, buffer, strlen(buffer)) == -1)
    {
        perror("Ecriture PASS serveur");
        close(sockServerCMD);
        goto close_client;
    }

    // 10) Lecture reponse 230
    ecode = read(sockServerCMD, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture reponse 230");
        close(sockServerCMD);
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("Recu du serveur: %s\n", buffer);

    // 11) Renvoi 230 au client
    if (write(descSockCOM, buffer, strlen(buffer)) == -1)
    {
        perror("Renvoi 230 au client");
        close(sockServerCMD);
        goto close_client;
    }

    // 12) Lecture commande SYST
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture SYST client");
        close(sockServerCMD);
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("Recu du client ftp (SYST): %s\n", buffer);

    // 13) Envoi SYST au serveur
    if (write(sockServerCMD, buffer, strlen(buffer)) == -1)
    {
        perror("Ecriture SYST serveur");
        close(sockServerCMD);
        goto close_client;
    }

    // 14) Lecture 215
    ecode = read(sockServerCMD, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture 215 serveur");
        close(sockServerCMD);
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("Recu du serveur (215): %s\n", buffer);

    // 15) Renvoi 215 au client
    if (write(descSockCOM, buffer, strlen(buffer)) == -1)
    {
        perror("Renvoi 215 client");
        close(sockServerCMD);
        goto close_client;
    }

    // 16) Lecture PORT x,x,x,x,x,x
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture PORT client");
        close(sockServerCMD);
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("Recu du client ftp (PORT): %s\n", buffer);

    // Découpage
    int n1, n2, n3, n4, n5, n6;
    if (sscanf(buffer, "PORT %d,%d,%d,%d,%d,%d", &n1, &n2, &n3, &n4, &n5, &n6) != 6)
    {
        fprintf(stderr, "Commande PORT mal formatée.\n");
        close(sockServerCMD);
        goto close_client;
    }
    char ipClient[50], portClient[10];
    sprintf(ipClient, "%d.%d.%d.%d", n1, n2, n3, n4);
    sprintf(portClient, "%d", (n5 * 256) + n6);

    // 17) Connexion en mode actif au client
    int actif;
    ecode = connect2Server(ipClient, portClient, &actif);
    if (ecode == -1)
    {
        perror("Connexion mode actif client");
        close(sockServerCMD);
        goto close_client;
    }

    // 18) Envoi PASV au serveur
    char command[50] = "PASV\r\n";
    if (write(sockServerCMD, command, strlen(command)) == -1)
    {
        perror("Ecriture PASV vers serveur");
        close(sockServerCMD);
        close(actif);
        goto close_client;
    }

    // 19) Lecture 227 (mode passif) du serveur
    ecode = read(sockServerCMD, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture 227 (PASV)");
        close(sockServerCMD);
        close(actif);
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("Recu du serveur (227): %s\n", buffer);

    if (sscanf(buffer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)",
               &n1, &n2, &n3, &n4, &n5, &n6) != 6)
    {
        fprintf(stderr, "Réponse PASV mal formatée.\n");
        close(sockServerCMD);
        close(actif);
        goto close_client;
    }
    char ipServeur[50], portServeur[10];
    sprintf(ipServeur, "%d.%d.%d.%d", n1, n2, n3, n4);
    sprintf(portServeur, "%d", (n5 * 256) + n6);

    // 20) Connexion au serveur en mode passif
    int passif;
    ecode = connect2Server(ipServeur, portServeur, &passif);
    if (ecode == -1)
    {
        perror("Connexion passif serveur");
        close(sockServerCMD);
        close(actif);
        goto close_client;
    }

    // ----------------------------
    //  TIMEOUT sur la socket passif
    // ----------------------------
    {
        struct timeval tv;
        tv.tv_sec  = 5;   // Timeout : 5 secondes
        tv.tv_usec = 0;
        if (setsockopt(passif, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
        {
            perror("setsockopt (SO_RCVTIMEO) a échoué");
        }
    }

    // 21) Envoi de 200 (OK) au client pour confirmer le PORT
    char message[50] = "200 PORT Command successful\r\n";
    if (write(descSockCOM, message, strlen(message)) == -1)
    {
        perror("Ecriture 200 vers client");
        close(sockServerCMD);
        close(actif);
        close(passif);
        goto close_client;
    }

    // 22) Lecture commande LIST du client
    ecode = read(descSockCOM, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture LIST du client");
        close(sockServerCMD);
        close(actif);
        close(passif);
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("Recu du client ftp (LIST): %s\n", buffer);

    // 23) Envoi LIST au serveur
    if (write(sockServerCMD, buffer, strlen(buffer)) == -1)
    {
        perror("Ecriture LIST serveur");
        close(sockServerCMD);
        close(actif);
        close(passif);
        goto close_client;
    }

    // 24) Lecture 150 du serveur
    ecode = read(sockServerCMD, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture 150 (serveur)");
        close(sockServerCMD);
        close(actif);
        close(passif);
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("Recu du serveur (150): %s\n", buffer);

    // 25) Envoi 150 au client
    if (write(descSockCOM, buffer, strlen(buffer)) == -1)
    {
        perror("Renvoi 150 au client");
        close(sockServerCMD);
        close(actif);
        close(passif);
        goto close_client;
    }

    // 26) Lecture sur la data-connection (passif) => écrit sur actif
    printf("Recu du serveur (listing):\n");
    while ((ecode = read(passif, buffer, MAXBUFFERLEN - 1)) > 0)
    {
        buffer[ecode] = '\0';
        printf("%s", buffer);

        if (write(actif, buffer, ecode) == -1)
        {
            perror("Ecriture socket actif");
            break;
        }
    }
    if (ecode < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            fprintf(stderr, "\nTimeout atteint, on quitte la lecture data.\n");
        else
            perror("\nErreur de lecture sur passif");
    }

    // Fermeture manuelle des sockets data
    close(actif);
    close(passif);

    // 27) Lecture 226 du serveur
    ecode = read(sockServerCMD, buffer, MAXBUFFERLEN - 1);
    if (ecode <= 0)
    {
        perror("Lecture 226");
        close(sockServerCMD);
        goto close_client;
    }
    buffer[ecode] = '\0';
    printf("\nRecu du serveur: %s\n", buffer);

    // 28) Renvoi 226 (ou autre code) au client
    if (write(descSockCOM, buffer, strlen(buffer)) == -1)
    {
        perror("Renvoi 226 au client");
    }

    // 29) Fermeture serveur
    close(sockServerCMD);

close_client:
    // 30) Fermeture socket client
    close(descSockCOM);
    // Retour au main
}
