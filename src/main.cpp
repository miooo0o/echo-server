#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <limits>
#include <sstream>


void	test_sock_value(sockaddr_in &address)
{
	std::cout << address.sin_family << std::endl;
	std::cout << address.sin_addr.s_addr << std::endl;
	std::cout << address.sin_port << std::endl;	
}

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
	hint.sin_port = htons(54000);
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
	char	host[NI_MAXHOST];
	char	svc[NI_MAXSERV];

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
		
	}
	return (EXIT_SUCCESS);
}