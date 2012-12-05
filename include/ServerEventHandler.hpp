/*
 * Date - 21/11/2012
 * Author - Garima Agarwal
 */

#include <memory>
#include "EventHandler.hpp"
#include "server_socket.hpp"

template <typename T, template <typename> class H>
class ServerSocketHandler: public EvHandler
{
    public:
      ServerSocketHandler(unique_ptr<ServerSocket<T>> sock)
      {
          sock_fd = std::move(sock);
          Reactor *rec = Reactor::getInstance();
          rec->registerHandler(EV_IN, (sock_fd.get())->getSockfd(), this);    
      }
      int handleRead();      
    
    private:
        unique_ptr<ServerSocket<T>> sock_fd;
        vector<unique_ptr<H<T>>> mapClient;
}; 

template <typename T, template <typename> class H>
int ServerSocketHandler<T, H>::handleRead()
{
    std::cout<<"Got a new connection"<<std::endl;
    std::unique_ptr<Socket<T>> newSock;
    newSock = (sock_fd.get())->acceptConn(std::move(newSock));
    unique_ptr<H<T>> sockHd(new H<T>(std::move(newSock)));
    mapClient.push_back(std::move(sockHd));
}
