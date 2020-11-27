#ifndef __REDIS_QUIC_H
#define __REDIS_QUIC_H

#include <msquic.h>
#include "connection.h"

typedef struct quicConnection
{
    connection conn;
    HQUIC connection;
    char *ip;
    char *port;
    
}quicConnection;

#endif