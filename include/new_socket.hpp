/*
 * Author - 
 *
 */

#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__
#include <sys/ioctl.h>
#include "include.hpp"
#include "exceptions.hpp"
#include "traits.hpp"
#include "base_address.hpp"

template<class T, class Enable = void>
class Socket;

template<class T>
class Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type> 
{
    protected:
        Socket(int fd): _sockfd(fd), _read_bytes(0), _write_bytes(0){}
    public:
        Socket();

        /*
         * Destructor for Socket class
         */
        ~Socket()
        {
            close(_sockfd);    
        }

        /*
         * read bytes from the socket
         */
        string& readBytes(){}
        
        
        int getSockfd() { return _sockfd; }
        /*
         * write bytes to the socket
         */
        int writeBytes(const string& data){}

        /*
         * Set socket to non-blocking mode
         */
        void setNonBlocking(){}

        void setKeepAlive(){}

        void setNoDelay(){}
        
    private:
        int _sockfd;
        int _read_bytes;
        int _write_bytes;
};

template<class T>
Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type>::Socket():_read_bytes(0), _write_bytes(0)
{
    std::cout<<"Calling base const,"<<T::_domain<<" "<<T::_type;
    _sockfd = socket(T::_type, T::_domain, 0);
    std::cout<<"Sock fd is "<<_sockfd;
}

using sock_stream = Socket<inet_stream_addr>; 
#endif
