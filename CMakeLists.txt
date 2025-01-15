cmake_minimum_required(VERSION 3.15)
project(Proxy C)

# Set the C standard
set(CMAKE_C_STANDARD 11)

# Create a library for simpleSocketAPI
add_library(apisocket simpleSocketAPI.c
        serveur.c)

# Create an executable target for the proxy
add_executable(proxy
        proxy.c
        proxyAPI.c
        proxyAPI.h
)

# Ajout du serveur
add_executable(serveur serveur.c simpleSocketAPI.c)

# Ajout du client
add_executable(client client.c simpleSocketAPI.c)

# Link the apisocket library to the proxy executable
target_link_libraries(proxy PRIVATE apisocket)
