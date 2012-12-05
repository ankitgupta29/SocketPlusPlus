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
#include<sys/ioctl.h>
#define BUFFERLENGTH 1024


template<class T, class Enable = void>
class Socket;

template<class T>
class Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type> 
{
    protected:
    public:
        Socket(int fd): _sockfd(fd), _read_bytes(0), _write_bytes(0){}
        Socket();

        /*
         * Destructor for Socket class
         */
        ~Socket()
        {
            close(_sockfd);    
        }

        /*
         * Copy constructor for sockets
         */
         Socket(const Socket<T>& other): _sockfd(other._sockfd), _read_bytes(other._read_bytes), _write_bytes(other._write_bytes)
         {
                
         }
        /*
         * read bytes from the socket
         */
        string readBytes();

        int getSockfd() 
        {
            return _sockfd;
        }
        /*
         * write bytes to the socket
         */
        int writeBytes(const string& data);

        /*
         * Set socket to non-blocking mode
         */
        void setNonBlocking(bool flag);

        void setKeepAlive(bool flag);

        void setBroadcast(bool flag);

        void setNoDelay(bool flag);

        void setReuseAddress(bool flag);

        void getKeepAlive();

        void socketClose();

    private:
        int _sockfd;
        int _read_bytes;
        int _write_bytes;

};//end of Class

    template<class T>
Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type>::Socket():_read_bytes(0), _write_bytes(0)
{
    std::cout<<"Calling base const,"<<T::_domain<<" "<<T::_type;
    _sockfd = socket(T::_type, T::_domain, 0);
    std::cout<<"Sock fd is "<<_sockfd;
}

template<class T>
string Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type>::readBytes()
{
    string data;
    char recBuffer[BUFFERLENGTH] = {0};
    int bytesAv = 0;
    int bytesRead = 0;
    int flags = 0;
    if ( ioctl (this->_sockfd,FIONREAD,&bytesAv) < 0 )
    {
        throw SockError("Error in Reading from Socket.");		
    }
    if ( bytesAv < 0 )
    {
        // No Data Available
        throw SockError("Error In Reading");	
    }

    if ( bytesAv == 0)
    {
        // No Data Available
        //sockState = socket_state::TIME_WAIT;
        throw SockError("No Data available for read");	
    }

    bytesRead = recv(this->_sockfd,recBuffer,BUFFERLENGTH,flags);
    (data) += recBuffer;

    return data;
}

template<class T>
int Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type>::writeBytes(const string& data) 
{
    const char* message = data.c_str(); // is this being lost ??
    int sent = 0, length = data.length();

    for(int i=length; i!=0 ; i-=sent ) 
    {
        sent = send(_sockfd, message + length - i, i, 0);
        if( sent < 1 )
            throw SockError("Message could not be sent.");
    }

}

template<class T>
void Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type>::socketClose() 
{ 	
    close(_sockfd); 
}


/*
   .. Set the socket I/O mode: In this case FIONBIO
   .. enables or disables the blocking mode for the 
   .. socket based on the numerical value of iMode.
   .. If flag = 0, blocking is enabled
   .. If flag = 1, non-blocking mode is enabled
 */
template<class T>
void Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type>::setNonBlocking(bool flag) 
{
    int iResult;
    iResult = ioctl(_sockfd, FIONBIO, &flag);
    if (iResult == -1)
    {
        throw SockError("ioctl failed with error in setting socket to non-blocking");
    }			
}


//Returns the value of the SO_KEEPALIVE socket option. 
template<class T>
void Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type>::getKeepAlive()  {
    int value = 0;
    int rc;
    rc = setsockopt(this->_sockfd, SOL_SOCKET,SO_KEEPALIVE, reinterpret_cast<const char*>(value),sizeof(value));
    if (rc == -1) 
    {
        throw SockError("Error in setting Socket Options");
    }    
}


//The SO_KEEPALIVE socket option is designed to allow an application 
//to enable keep-alive packets for a socket connection
template<class T>
void Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type>::setKeepAlive(bool flag)  {	
    int value = flag ? 1 : 0;
    int rc;
    rc = setsockopt(this->_sockfd, SOL_SOCKET,SO_KEEPALIVE, reinterpret_cast<const char*>(value),sizeof(value));
    if (rc == -1) 
    {
        throw SockError("Error in setting Socket Options");
    }
}

/*
   ..TCP_NODELAY is for a specific purpose; to disable the Nagle buffering
   ..algorithm. It should only be set for applications that send frequent
   ..small bursts of information without getting an immediate response,
   ..where timely delivery of data is required (the canonical example is
   ..mouse movements)
 */
template<class T>
void Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type>::setNoDelay(bool flag)  {
    //Returns the value of the TCP_NODELAY socket option.
    int value = flag ? 1 : 0;
    int rc;
    rc = setsockopt(this->_sockfd, IPPROTO_TCP,TCP_NODELAY,reinterpret_cast<const char*>(value),sizeof(value));
    if (rc == -1) 
    {
        throw SockError("Error in setting Socket Options");
    }
}

/*
   ..This socket option tells the kernel that even if this port is busy 
   ..(in the TIME_WAIT state), go ahead and reuse it anyway.If it is busy,
   ..but with another state, you will still get an address already in use
   ..error.It is useful if your server has been shut down, and then
   ..restarted right away while sockets are still active on its port
 */
template<class T>
void Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type>::setReuseAddress(bool flag)
{
    int value = flag ? 1 : 0;
    int rc;
    rc = setsockopt(this->_sockfd, SOL_SOCKET,SO_REUSEADDR,reinterpret_cast<const char*>(value),sizeof(value));
    if (rc == -1) 
        throw SockError("Error in setting Socket Options");

}

template<class T>
void Socket<T, typename std::enable_if<Convertible<T*, base_addr*>()>::type>::setBroadcast(bool flag)
{
    int value = flag ? 1 : 0;
    int rc;

    rc = setsockopt(this->_sockfd, SOL_SOCKET,SO_BROADCAST,reinterpret_cast<const char*>(value),sizeof(value));
    if (rc == -1) 
    {
        throw SockError("Error in setting Socket Options");
    }
}

using sock_stream = Socket<inet_stream_addr>; 
#endif
