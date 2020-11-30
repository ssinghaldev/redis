#ifndef __REDIS_QUIC_H
#define __REDIS_QUIC_H

#include <msquic.h>
#include "connhelpers.h"
#include "server.h"

ConnectionType CT_QUIC;

typedef struct quicConnection {
    connection conn;
    HQUIC quic_conn_handle;
} quicConnection;

// TODO: Move all functions to quic.c file
connection *connCreateQuic(void) {
    quicConnection *quic_conn = zcalloc(sizeof(quicConnection));
    quic_conn->conn.fd = -1;
    quic_conn->conn.type = &CT_QUIC;
    quic_conn->quic_conn_handle = NULL;
    return (connection *) quic_conn;
}

connection *connCreateAcceptedQuic(HQUIC quic_connection)
{
    quicConnection *quic_conn = (quicConnection *) connCreateQuic();
    quic_conn->quic_conn_handle = quic_connection;
    quic_conn->conn.state = CONN_STATE_ACCEPTING;
    return (connection *) quic_conn;
}

//TODO: Design Stream/Connection Shutdown/Close cases and then populate the func accordingly
static void connQuicClose(connection *conn) {
    quicConnection *quic_conn = (quicConnection *) conn;
    QUIC_STATUS Status;

    if(quic_conn->quic_conn_handle){
        if(QUIC_FAILED(Status = server.cluster->quic_handlers.msquic->ConnectionShutDown(
                                quic_conn->quic_conn_handle,
                                QUIC_CONNECTION_SHUTDOWN_FLAG_NONE,
                                0))){
            serverLog(LL_WARNING, "Unable to shutdown the connection 0x%x!\n", Status);
        }
    }
}

// TODO: Design what should we do when there are errors during connections/stream
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

ConnectionType CT_QUIC = {
    .ae_handler = NULL,
    .close = connQuicClose,
    .write = NULL,
    .read = NULL,
    .accept = NULL,
    .connect = NULL,
    .set_write_handler = NULL,
    .set_read_handler = NULL,
    .get_last_error = connQuicGetLastError,
    .blocking_connect = NULL,
    .sync_write = NULL,
    .sync_read = NULL,
    .sync_readline = NULL,
    .get_type = connQuicGetType
};

#endif