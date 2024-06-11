#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <sstream>
#include <vector>
#include <thread>

class Server
{
public:
    Server(int port);
    ~Server();
    bool    start();
    void    run();

private:
    bool    createSocket();
    bool    bindSocket() const;
    bool    listenSocket() const;
    int     acceptConnection();
    void    handleClient(int clientSocket);

    int                     port_;
    int                     listening_;
    sockaddr_in             hint_;
    std::vector<pthread_t>  threads_;
};

#endif //SERVER_H
