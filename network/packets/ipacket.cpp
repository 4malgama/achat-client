#include "ipacket.h"

IPacket::IPacket(quint16 id)
{
	this->id = id;
}

IPacket::~IPacket()
{

}

quint16 IPacket::getId() const
{
	return id;
}
