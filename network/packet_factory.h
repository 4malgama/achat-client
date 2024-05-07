#ifndef PACKETFACTORY_H
#define PACKETFACTORY_H

#include <memory>
#include <cstdint>

class IPacket;

class PacketFactory
{
public:
	PacketFactory() = delete;
	PacketFactory(const PacketFactory&) = delete;
	PacketFactory(PacketFactory&&) noexcept = delete;

	static std::unique_ptr<IPacket> createPacket(uint32_t id);
};

#endif // PACKETFACTORY_H
