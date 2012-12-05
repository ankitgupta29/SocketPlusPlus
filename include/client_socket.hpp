#ifndef __CLIENT_SOCKET_HPP__
#define __CLIENT_SOCKET_HPP__

#include "new_socket.hpp"

template <typename T>
class ClientSocket: public Socket<T,typename std::enable_if<Convertible<T*, base_addr*>()>::type>
{
    public:
        ClientSocket(unsigned int server_port = 80,string ipaddress="localhost")
        {
            buildClientSocket();
        }
        ClientSocket(string server_port = "http",string ipaddress="localhost")
        {
            buildClientSocket();
        }

        // Destructor
        ~ClientSocket() {}

    private:
        T _ipaddress;
        int _server_port;
        void buildClientSocket();

};

template <typename T>
void ClientSocket<T>::buildClientSocket() 
{ 	
	addrinfo* r = NULL;
	int socketID;
	T addr(atoi((char*)_server_port),_ipaddress);
	for( r = addr.getResult(); r != NULL; r = r->ai_next) 
	{
		socketID = socket( r->ai_family, r->ai_socktype, r->ai_protocol );
		if ( socketID == -1 ) {
			throw SockError("Error on creating socket"); 
			continue;
		}
		if (connect(socketID, r->ai_addr, r->ai_addrlen) == -1) {
			close(socketID);
			throw SockError("Error in connect"); 
			continue;
		}
		break;
	}

	if (r == NULL) 
	{
		throw SockError("Error in address"); 
		socketID = -1;
	}
}

#endif
