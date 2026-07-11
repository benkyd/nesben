#pragma once

#include <cstdint>
#include <tuple>
#include <memory>
#include <vector>

class BusItem
{
public:
    // Start address, size
    virtual std::tuple<uint8_t, uint8_t> Allocate();

    virtual uint8_t Read(uint16_t addr) = 0;
    virtual void Write(uint16_t addr) = 0;
};

class Bus
{
public:
    Bus();
    Bus(std::unique_ptr<BusItem> item...);
    ~Bus();

    void AddItem(std::unique_ptr<BusItem> item);

    uint8_t Read(uint16_t addr);
    void Write(uint16_t addr);
};

