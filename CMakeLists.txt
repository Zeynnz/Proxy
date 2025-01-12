cmake_minimum_required(VERSION 3.15)
project(Proxy C)

# Set the C standard
set(CMAKE_C_STANDARD 11)

# Create a library for simpleSocketAPI
add_library(apisocket CodeProxy/simpleSocketAPI.c
        CodeProxy/serveur.c)

# Create an executable target for the proxy
add_executable(proxy
        CodeProxy/proxy.c
        CodeProxy/proxyAPI.c
        CodeProxy/proxyAPI.h
)

# Ajout du serveur
add_executable(serveur CodeProxy/serveur.c CodeProxy/simpleSocketAPI.c)

# Ajout du client
add_executable(client CodeProxy/client.c CodeProxy/simpleSocketAPI.c)

# Link the apisocket library to the proxy executable
target_link_libraries(proxy PRIVATE apisocket)
