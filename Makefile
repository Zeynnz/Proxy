# Compilation des exécutables
all: client serveur

client: client.o simpleSocketAPI.o
	gcc $^ -o $@

serveur: serveur.o simpleSocketAPI.o
	gcc $^ -o $@

# Règles de compilation des objets
client.o: client.c simpleSocketAPI.h
	gcc -c $^ -o $@

serveur.o: serveur.c simpleSocketAPI.h
	gcc -c $^ -o $@

simpleSocketAPI.o: simpleSocketAPI.c simpleSocketAPI.h
	gcc -c $^ -o $@

# Nettoyage des fichiers générés
clean:
	rm -f client serveur *.o
