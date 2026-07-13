#pragma once

#include <cstdint>
#include <tuple>
#include <memory>
#include <vector>

class BusClient
{
public:
    // Start address, size
    virtual std::tuple<uint16_t, uint16_t> DeclaredRange();

    virtual uint8_t Read(uint16_t addr) = 0;
    virtual void Write(uint16_t addr, uint8_t val) = 0;
};

class Bus
{
public:
    Bus();
    Bus(std::unique_ptr<BusClient> clients...);
    ~Bus();

    void AddClient(std::unique_ptr<BusClient> client);

    void TickForEach();

    uint8_t Read(uint16_t addr);
    void Write(uint16_t addr, uint8_t val);

private:
    uint8_t mYoloModule;
};

