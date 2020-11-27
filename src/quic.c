#include "quic.h"

QUIC_STATUS ServerListenerCallback(HQUIC listener, void* context, QUIC_LISTENER_EVENT* Event)
{
    return QUIC_STATUS_SUCCESS;
}