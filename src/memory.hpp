#pragma once

#include <cstdint>

#include "bus.hpp"

template <uint16_t MemoryStart, uint16_t MemorySize>
class Memory : public BusItem
{
public:
    Memory();
    ~Memory();

    uint8_t Read(uint16_t addr) override;
    void Write(uint16_t addr) override;

private:
    uint8_t data[MemorySize];
};

