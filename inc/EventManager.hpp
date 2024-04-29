#ifndef WEBSERV_EVENT_MANAGER_H
#define WEBSERV_EVENT_MANAGER_H

#if defined(__MACH__)
#include "EventManagerKqueue.hpp"
typedef EventManagerKqueue EventManager;
#elif defined(__linux__)
#include "EventManagerEpoll.hpp"
typedef EventManagerEpoll EventManager;
#endif

#endif
