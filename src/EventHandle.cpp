#include<iostream>
#include<map>
#include<sys/select.h>
#include<memory>
#include"EventHandler.hpp"

Reactor* Reactor::getInstance() 
{
    if (ins == NULL) {
        ins = new Reactor();
    }
    return ins;
}

Reactor::Reactor(): maxfd(0) 
{   
    timeout.tv_sec  = TIMEOUT * 60;
    timeout.tv_usec = 0;
    FD_ZERO(&masterReadSet);
    FD_ZERO(&masterWriteSet);
}

bool Reactor::registerHandler(Events ev, int fd, EvHandler *evH) 
{
    switch (ev)
    {
        case EV_IN:
            FD_SET(fd, &masterReadSet);
            fdReadMap.insert(std::pair<int, EvHandler*>(fd, evH));
            break;
        case EV_OUT:
            FD_SET(fd, &masterWriteSet);
            fdReadMap.insert(std::pair<int, EvHandler*>(fd, evH));
            break;
    }
    if(fd > maxfd) 
    {
        maxfd = fd;
    }
    return true;
}

bool Reactor::registerTimeoutHandler(EvHandler *evH) 
{
    timoutHandler = evH;
}

bool Reactor::deregisterHandler(Events ev, int fd, EvHandler *evH) 
{
    std::map<int, EvHandler*>::iterator iter;
    switch (ev) 
    {
        case EV_IN:
            FD_CLR(fd, &masterReadSet);
            iter = fdReadMap.find(fd);
            fdReadMap.erase(iter);
            break;
        case EV_OUT:
            FD_CLR(fd, &masterWriteSet);
            iter = fdWriteMap.find(fd);
            fdWriteMap.erase(iter);
            break;
    }
    if (fd == maxfd) 
    {
        while (FD_ISSET(maxfd, &masterWriteSet) == false || FD_ISSET(maxfd, &masterReadSet) == false) 
        {
            maxfd -= 1;
        }
    }
    return true;
}

void Reactor::Run() 
{
    int rc;
    fd_set workingReadSet, workingWriteSet;
    FD_ZERO(&workingReadSet);
    FD_ZERO(&workingWriteSet);
    do 
    {
        memcpy(&workingReadSet, &masterReadSet, sizeof(fd_set));
        memcpy(&workingWriteSet, &masterWriteSet, sizeof(fd_set));

        rc = select(maxfd + 1, &workingReadSet, NULL, NULL, NULL);

        if (rc < 0)
        {
            perror("  select() failed");
            break;
        }

        if (rc == 0)
        {
            printf("  select() timed out\n");
            timoutHandler->handleTimeout();
            continue;
        }

        int totalfds = rc;
        for (int i=0; i <= maxfd  && totalfds > 0; ++i)
        {
            if (FD_ISSET(i, &workingReadSet))
            {
                fdReadMap[i]->handleRead();
                totalfds--;
            } 

            if (FD_ISSET(i, &workingWriteSet)) 
            {
                fdReadMap[i]->handleWrite();
                totalfds--;
            }
        }
    } while(1);
}

Reactor* Reactor::ins = NULL;

