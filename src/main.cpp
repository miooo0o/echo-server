#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <limits>
#include <sstream>


int	main(int argc, char** argv)
{
	// 0. Verify valid argument values
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
		return (EXIT_FAILURE);
	}

	std::istringstream	iss(argv[1]);
	int					port;

	if (!(iss >> port) || !(iss.eof()) || port < 1 || port > 65535)
	{
		std::cerr << "Invalid port number: " << argv[1] << std::endl;
		return (EXIT_FAILURE);
	}
	
	// 1. socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	
	// 2. bind
	sockaddr_in address =
	{
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons((port))
	};
	
	// 3. listen
	// 4. accept
	// 5. recv
	// 6. send
	// 7. goto 5
	
	return (EXIT_SUCCESS);
}