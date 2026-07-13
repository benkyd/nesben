#include "bus.hpp"

#include <yolo.hpp>

Bus::Bus()
{
    mYoloModule = yolo::registerModule("BUS", "\e[0;33m");
    yolo::info(mYoloModule, "BUS UP");
}

Bus::Bus(std::unique_ptr<BusClient> clients...)
{

    mYoloModule = yolo::registerModule("BUS", "\e[0;33m");
    yolo::info(mYoloModule, "BUS UP");
}

Bus::~Bus()
{

}

void Bus::AddClient(std::unique_ptr<BusClient> client)
{

}

void Bus::TickForEach()
{

}

uint8_t Bus::Read(uint16_t addr)
{
    return 0xFF;
}

void Bus::Write(uint16_t addr, uint8_t val)
{

}

