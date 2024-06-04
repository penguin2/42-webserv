#ifndef WEBSERV_CONNECTION_STATE_H
#define WEBSERV_CONNECTION_STATE_H

namespace connection {

typedef enum eState { RECV, SEND, CGI, CGI_TIMEOUT, CGI_ERROR, CLOSED } State;

}

#endif
