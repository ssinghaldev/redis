#ifndef __REDIS_QUIC_H
#define __REDIS_QUIC_H

#include <msquic.h>

// typedef struct quicHandlers
// {
//     QUIC_API_TABLE* msquic;
//     HQUIC registration;
//     // QUIC_REGISTRATION_CONFIG reg_config;
//     HQUIC server_configuration;
//     HQUIC client_configuration;
//     HQUIC listener;
// }quicHandlers;

typedef struct quicConnection
{
    HQUIC connection;
    char *ip;
    char *port;
    
}quicConnection;

// int quicInit(quicHandlers *handlers, int port);

QUIC_STATUS ServerListenerCallback(HQUIC listener, void* context, QUIC_LISTENER_EVENT* Event);

#endif