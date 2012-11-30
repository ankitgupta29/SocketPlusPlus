#ifndef __SERVER_SOCKET_HPP__
#define __SERVER_SOCKET_HPP__

#include "new_socket.hpp"

#define MAX_LISTEN 20

template <typename T>
class ServerSock: public Socket<T,typename std::enable_if<Convertible<T*, base_addr*>()>::type>
{
    public:
        ServerSock(int listen_port);
        ServerSock(int listen_port, int max_listen);
        Socket<T> acceptConn();

    private:
        T _address;
        int _listen_port;
        int _max_listen; 
};


template <typename T>
ServerSock<T>::ServerSock(int listen_port):_listen_port(listen_port), _max_listen()
{
    T addr("localhost");
    struct addrinfo *p = addr.getResult();
    for(p = addr.getResult(); p != NULL; p = p->ai_next) 
    {
        std::cout<<"Addr "<<p->ai_addrlen<<std::endl;
        if (bind(this->getSockfd(), p->ai_addr, p->ai_addrlen) == -1)
        {
            std::cout<<"Bind error";
            continue;
        }
    }

    if (p == NULL)
    {
        //bind error, throw expection    
    }

    if (listen(this->getSockfd(), _max_listen) < 0)
    {
        std::cout<<"Listen error";
        //throw exception    
    }
}

using server_sock_stream = ServerSock<inet_stream_addr>;
#endif 
