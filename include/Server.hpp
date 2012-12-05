#include "include.hpp"
#include "ServerEventHandler.hpp"


template <typename T, template <typename> class H, template <typename, template <typename> class> class S>
class AsyncServer
{
    public:
        AsyncServer(int listenPort);
    
    private:
        unique_ptr<ServerSocket<T>> listen_sock;
        S<T, H> serverHandler;
};

template <typename T, template <typename> class H, template <typename, template <typename> class> class S>
AsyncServer<T, H, S>::AsyncServer(int listenPort): listen_sock(new ServerSocket<T>(listenPort)), serverHandler(std::move(listen_sock))
{
}
