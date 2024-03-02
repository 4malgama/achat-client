#ifndef PACKETFACTORY_H
#define PACKETFACTORY_H

#include <qglobal.h>

class IPacket;

class PacketFactory
{
public:
	PacketFactory() = delete;
	PacketFactory(const PacketFactory&) = delete;
	PacketFactory(PacketFactory&&) noexcept = delete;

	static IPacket *makePacket(quint32 id);
};

#endif // PACKETFACTORY_H
