#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>

int	main(int argc, char** argv)
{

	// Verify valid argument values
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		return (EXIT_FAILURE);
	}
	
	std::istringstream	iss(argv[1]);
	int					port;
	
	// Check if the port number is valid
	if (!(iss >> port) || !(iss.eof()) || port < 1 || port > 65535)
	{
		std::cerr << "Invalid port number: " << argv[1] << std::endl;
		return (EXIT_FAILURE);
	}
	std::cout << "Port number: " << port << std::endl;
	
	const int	listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1)
	{
		std::cerr << "Can't creat a socket";
		return -1;
	}

	sockaddr_in	hint = {};
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	
	if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
	{
		std::cerr << "Can't bind IP/Port";
		return -2;
	}

	if (listen(listening, SOMAXCONN) == -1)
	{
		std::cerr << "Can't listen";
		return -3;
	}

	sockaddr_in	client;
	socklen_t	clientSize = sizeof(client);
	char		host[NI_MAXHOST];
	char		svc[NI_MAXSERV];

	int clientSocket = accept(listening, (sockaddr*)&hint, &clientSize);
	if (clientSocket == -1)
	{
		std::cerr << "Problem with Client connecting";
		return -4;
	}
	close(listening);
	
	memset(host, 0, NI_MAXHOST);
	memset(svc,0, NI_MAXSERV);
	
	int result = getnameinfo((sockaddr*)&client, clientSize,
								host, NI_MAXHOST,
								svc, NI_MAXSERV,
								0);

	if (result)
	{
		std::cout<< host << " connected on " << svc << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on" << ntohs(client.sin_port) << std::endl;
	}

	char buf[4096];
	while (true)
	{
		// clear the buffer
		memset(buf, 0, 4096);
		
		// wait for a message
		int bytesRecv = recv(clientSocket, buf, 4096, 0);
		if (bytesRecv == -1)
		{
			std::cerr << "there was a connection issue" << std::endl;
			break ;
		}
		if (bytesRecv == 0)
		{
			std::cout << "the client disconnected" << std::endl;
			break ;
		}
		// display the message
		std::cout << "Recived: " << std::string(buf, 0, bytesRecv) << std::endl;

		// prepare the message to be sent back
		std::string response = "Echo: " + std::string(buf, 0, bytesRecv);
		
		// resend the message
		int bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
		if (bytesSent == -1)
		{
			std::cerr << "Error sending message back to client" << std::endl;
			break;
		}
		// display the sent message
		std::cout << "Sent: " << response;
	}
	close(clientSocket);
	
	return (EXIT_SUCCESS);
}