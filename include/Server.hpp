#include "ServerEventHandler.cpp"


template <typename T, typename S,typename H>
class AsyncServer
{
    public:
        AsyncServer(int listenPort);
    
    
    private:
        ServerSock<T> listen_sock;
        ServerSocketHandler<T> serverHandler;
}

template <typename T>
AsyncServer::AsyncServer(int listenPort)
{
    
    
}
