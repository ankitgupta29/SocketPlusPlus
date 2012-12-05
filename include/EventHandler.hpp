/*
 *	Date - 21/11/2012	
 *  Author - Garima Agarwal
 *	EventHandler file
 */
#ifndef __EVENT_HANDLER_HPP
#define __EVENT_HANDLER_HPP
#include<iostream>
#include<map>
#include<sys/select.h>
#include<memory>

#include "new_socket.hpp"
#include "base_address.hpp"

enum Events {
	EV_IN,
	EV_OUT,
	EV_ERR,
	EV_HUP	
};

const int MAX_FD = 64 * 1024;
const int TIMEOUT = 3;
class EvHandler
{
	public:
		EvHandler()
		{
		}

		virtual int handleRead(){};
		virtual int handleWrite(){};
		virtual int handleTimeout(){};
		
		~EvHandler(){};
};

class Reactor 
{
    public:
        bool registerHandler(Events ev, int fd, EvHandler *evH); 
        bool deregisterHandler(Events ev, int fd, EvHandler *evH); 
        bool registerTimeoutHandler(EvHandler *evH); 
        //run it in a different thread since it is blocking
        void Run();
        static Reactor *getInstance();

    private:
        Reactor();
        std::map<int, EvHandler*> fdReadMap;
        std::map<int, EvHandler*> fdWriteMap;
        EvHandler *fdReadArr[MAX_FD];    
        EvHandler *fdWriteArr[MAX_FD];
        fd_set masterReadSet, masterWriteSet;
        int maxfd;
        struct timeval timeout;
        EvHandler *timoutHandler;
        static Reactor *ins;
};

template <typename T>
class SocketHandler: public EvHandler 
{
    public:
        SocketHandler(unique_ptr<Socket<T>> sock)
        {
            sock_fd = std::move(sock);
            Reactor *rec = Reactor::getInstance();
            rec->registerHandler(EV_IN, (sock_fd.get())->getSockfd(), this);
            //rec->registerHandler(EV_OUT, (sock_fd.get())->getSockfd(), this);    
        };

        int handleRead()
        {
            string data = ((this->sock_fd).get())->readBytes();
            std::cout<<data;   
            ((this->sock_fd).get())->writeBytes("hi");
        }

        int handleWrite(){};
    protected:
        std::string data;
        unique_ptr<Socket<T>> sock_fd;
};

#endif
