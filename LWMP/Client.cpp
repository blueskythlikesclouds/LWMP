		#include "Client.h"
#include <WS2tcpip.h>

Client::Client() : Socket(SOCK_DGRAM, IPPROTO_UDP)
{
}
