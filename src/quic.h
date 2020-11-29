#ifndef __REDIS_QUIC_H
#define __REDIS_QUIC_H

#include <msquic.h>
#include "connhelpers.h"
#include "server.h"

typedef struct quicConnection
{
    connection conn;
    HQUIC quic;
    QUIC_NEW_CONNECTION_INFO info;   
}quicConnection;

ConnectionType CT_QUIC;

// TODO: Move all functions to quic.c file
connection *connCreateQuic(void)
{
    quicConnection *quic_conn = zcalloc(sizeof(quicConnection));
    quic_conn->conn.fd = -1;
    quic_conn->conn.type = &CT_QUIC;
    return (connection *) quic_conn;
}

connection *connCreateAcceptedQuic(HQUIC quic_connection, QUIC_NEW_CONNECTION_INFO *info)
{
    quicConnection *quic_conn = (quicConnection *) connCreateQuic();
    quic_conn->quic = quic_connection;
    quic_conn->info = *info;
    quic_conn->conn.state = CONN_STATE_ACCEPTING;
    return (connection *) quic_conn;
}

// TODO: Check if we need to do something here.
static int connQuicConnect(connection *conn, const char *addr, int port, const char *src_addr,
        ConnectionCallbackFunc connect_handler)
{
    quicConnection *quic_conn = (quicConnection *) conn;
    return C_OK;
}

// TODO: Implement stream write functions here.
static int connQuicWrite(connection *conn, const void *data, size_t data_len) 
{
    quicConnection *quic_conn = (quicConnection *) conn;
    return C_OK;
}

static void connQuicClose(connection *conn)
{
    quicConnection *quic_conn = (quicConnection *) conn;
    server.cluster->quic_handlers.msquic->ConnectionClose(quic_conn->quic);
    zfree(quic_conn);
}

// TODO: Point it to Quic error status instead of conn last error.
static const char *connQuicGetLastError(connection *conn) 
{
    quicConnection *quic_conn = (quicConnection *) conn;
    return strerror(quic_conn->conn.last_errno);
}

static int connQuicGetType(connection *conn) 
{
    (void) conn;
    return CONN_TYPE_QUIC;
}

// TODO: See if other handlers are required or not. 
// Ideally, the other handlers should not be called for QUIC connections.
ConnectionType CT_QUIC = {
    .connect = connQuicConnect,
    .write = connQuicWrite,
    .close = connQuicClose,
    .get_last_error = connQuicGetLastError,
    .get_type = connQuicGetType
};

#endif