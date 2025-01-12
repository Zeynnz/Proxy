CC=gcc

# Fichiers sources pour les exécutables
CLIENT_SOURCES = client.c simpleSocketAPI.c
SERVER_SOURCES = serveur.c simpleSocketAPI.c

# Noms des exécutables
CLIENT_EXEC = client
SERVER_EXEC = serveur

# Fichiers objets
CLIENT_OBJECTS = client.o simpleSocketAPI.o
SERVER_OBJECTS = serveur.o simpleSocketAPI.o

# Compilation des exécutables
all: ${CLIENT_EXEC} ${SERVER_EXEC}

${CLIENT_EXEC}: ${CLIENT_OBJECTS}
	${CC} $^ -o $@

${SERVER_EXEC}: ${SERVER_OBJECTS}
	${CC} $^ -o $@

# Règles de compilation des objets
client.o: client.c simpleSocketAPI.h
	${CC} -c $^ -o $@

serveur.o: serveur.c simpleSocketAPI.h
	${CC} -c $^ -o $@

simpleSocketAPI.o: simpleSocketAPI.c simpleSocketAPI.h
	${CC} -c $^ -o $@

# Nettoyage des fichiers générés
clean:
	rm -f ${CLIENT_EXEC} ${SERVER_EXEC} *.o
