#ifndef PROXYAPI_H
#define PROXYAPI_H

#define BUFFER_SIZE 1024

void handleFTP(int clientSock);
void handlePortCommand(int clientSock, int serverSock, char *buffer);

#endif //PROXYAPI_H
