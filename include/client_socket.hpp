#ifndef __CLIENT_SOCKET_HPP__
#define __CLIENT_SOCKET_HPP__

#include "socket.hpp"

template <typename T>
class client_socket: public Socket<T,typename std::enable_if<Convertible<T*, base_addr*>()>::type>
{
    public:
        client_socket(unsigned int server_port = 80,string ipaddress="localhost")
        {
            build_client_socket();
        }
        client_socket(string server_port = "http",string ipaddress="localhost")
        {
            build_client_socket();
        }

        // Destructor
        ~client_socket() {}

    private:
        T _ipaddress;
        int _server_port;
        void build_client_socket();

};

template <typename T>
void client_socket<T>::build_client_socket() 
{ 	
	addrinfo* r = NULL;
	int socketID;
	T addr(atoi((char*)_server_port),_ipaddress);
	for( r = addr.get_result(); r != NULL; r = r->ai_next) 
	{
		socketID = socket( r->ai_family, r->ai_socktype, r->ai_protocol );
		if ( socketID == -1 ) {
			throw sock_error("Error on creating socket"); 
			continue;
		}
		if (connect(socketID, r->ai_addr, r->ai_addrlen) == -1) {
			close(socketID);
			throw sock_error("Error in connect"); 
			continue;
		}
		break;
	}

	if (r == NULL) 
	{
		throw sock_error("Error in address"); 
		socketID = -1;
	}
}

#endif
