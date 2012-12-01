#ifndef __SERVER_SOCKET_HPP__
#define __SERVER_SOCKET_HPP__

#include "new_socket.hpp"

#define MAX_LISTEN 20

template <typename T>
class ServerSocket: public Socket<T,typename std::enable_if<Convertible<T*, base_addr*>()>::type>
{
    public:
        //ServerSock(int listen_port);
        ServerSocket(unsigned int listen_port,string ipaddress="localhost", int max_listen = 10);
        ServerSocket(string listen_port,string ipaddress="localhost", int max_listen = 10);
        Socket<T> acceptConn();
        
        // Destructor
		~ServerSocket() {}
		
    private:
        T _ipaddress;
        int _listen_port;
        int _max_listen; 

};

//if Port number is int
template <typename T>
ServerSocket<T>::ServerSocket(unsigned int listen_port, string ipaddress,int max_listen):_ipaddress(ipaddress),_listen_port(listen_port), _max_listen(max_listen)
{
    T addr(_listen_port,_ipaddress);
    struct addrinfo *p = addr.getResult();
    for(p = addr.getResult(); p != NULL; p = p->ai_next) 
    {
        std::cout<<"Addr "<<p->ai_addrlen<<std::endl;
        if (bind(this->getSockfd(), p->ai_addr, p->ai_addrlen) == -1)
        {
        	close(this->getSocketfd());
            throw SockError("Error in Bind");
            continue;
        }
    }

    if (p == NULL)
    {
        //bind error, throw expection    
    }

    if (listen(this->getSockfd(), _max_listen) < 0)
    {
        throw SockError("Error in Listen."); 
    }
}


//If port number given in String    
template <typename T>
ServerSocket<T>::ServerSocket(string listen_port,string ipaddress, int max_listen):_ipaddress(ipaddress),_listen_port(atoi((char*)listen_port)), _max_listen(max_listen)
{
    T addr(_listen_port,_ipaddress);
    struct addrinfo *p = addr.getResult();
    for(p = addr.getResult(); p != NULL; p = p->ai_next) 
    {
        std::cout<<"Addr "<<p->ai_addrlen<<std::endl;
        if (bind(this->getSockfd(), p->ai_addr, p->ai_addrlen) == -1)
        {
        	close(this->getSocketfd());
            throw SockError("Error in Bind");
            continue;
        }
    }
    
    if (p == NULL)
    {
        //bind error, throw expection    
    }

    if (listen(this->getSockfd(), _max_listen) < 0)
    {
        throw SockError("Error in Listen");  
    }
}


template <typename T>
Socket<T> ServerSocket<T>::acceptConn()
{
  unsigned int newsockfd = accept(this->getSockfd());
  if (newsockfd < 0) 
         throw SockError("Error in Accept"); 
  
  Socket<T> acceptSocket(newsockfd);       
  return acceptSocket;
}


using server_sock_stream = ServerSocket<inet_stream_addr>;
#endif 
