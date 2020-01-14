#include "PacketHandler.h"

PacketHandler::PacketHandler(Address& address, Socket* socket) : stop(false), address(address), socket(socket)
{
	thread = std::thread(&PacketHandler::threadImplementation, this);
}

Address PacketHandler::getAddress() const
{
	return address;
}

void PacketHandler::setAddress(Address& address)
{
	this->address = address;
}

PacketHandler::~PacketHandler()
{
	stop = true;
	thread.join();
}
