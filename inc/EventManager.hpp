#ifndef WEBSERV_EVENT_MANAGER_H
#define WEBSERV_EVENT_MANAGER_H

#ifdef WEBSERV_MACOS
#include "EventManagerKqueue.hpp"
typedef EventManagerKqueue EventManager;
#else
#include "EventManagerEpoll.hpp"
typedef EventManagerEpoll EventManager;
#endif

#endif
