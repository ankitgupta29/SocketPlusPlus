/*
 * Date - 21/11/2012
 * Author - Garima Agarwal
 */

#include "EventHandler.hpp"

template <typename T>
class ServerSocketHandler: public EvHandler
{
    public:
      ServerSocketHandler(Socket<T> sock): sock_fd(sock)
      {
          Reactor *rec = Reactor::getInstance();
          rec->registerHandler(EV_IN, sock_fd, this);    
      }
      int handleRead();      
    
    private:
        
        ServerSock<T> sock_fd;
         
} 

template <typename T>
int ServerSocketHandler<T>:: handleRead()
{
    Socket<T> newSock = sock_fd.accept();
}
