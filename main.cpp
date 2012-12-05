//#include "server_socket.hpp"
#include "Server.hpp"
#include "client_socket.hpp"

template <typename T>
class handle: public SocketHandler<T>
{
   public:
    handle(unique_ptr<Socket<T>> sock): SocketHandler<T>(std::move(sock)){};      
    int handleRead()
    {
        SocketHandler<T>::handleRead();
        std::cout<<this->read_data;        
        write("hello");
    }

    void write(string data)
    {
        this->write_data = data;
        SocketHandler<T>::write();
    }
};

int main()
{
    Reactor *rec = Reactor::getInstance();
    AsyncServer<inet_stream_addr, handle, ServerSocketHandler> server(8080);   
   // unique_ptr<client_sock_stream> cl(new  client_sock_stream(80, "localhost"));
    /*handle<client_sock_stream> hd(cl);
    hd.write("hi");   */

    rec->Run();
    return 0;
}
