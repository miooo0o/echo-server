#include "Server.h"

Server::Server(int port) : port_(port), listening_(-1), hint_()
{
    hint_ = {};
    hint_.sin_family = AF_INET;
    hint_.sin_port = htons(port_);
    inet_pton(AF_INET, "0.0.0.0", &hint_.sin_addr);
}

Server::~Server()
{
    if (listening_ != -1)
        close(listening_);
}

bool Server::createSocket()
{
    listening_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_ == -1)
    {
        std::cerr << "Can't create a socket." << std::endl;
        return (false);
    }
    return (true);
}

bool Server::bindSocket() const
{
    if (bind(listening_, (sockaddr*)&hint_, sizeof(hint_)) == -1)
    {
        std::cerr << "Can't bind IP/Port" << std::endl;
        return (false);
    }
    return (true);
}

bool Server::listenSocket() const
{
    if (listen(listening_, SOMAXCONN) == -1)
    {
        std::cerr << "Can't listen" << std::endl;
        return (false);
    }
    return (true);
}

bool Server::start()
{
    if (!createSocket())
        return (false);
    if (!bindSocket())
        return (false);
    if (!listenSocket())
        return (false);
    return (true);
}


