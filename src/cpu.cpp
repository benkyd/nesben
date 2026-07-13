#include "cpu.hpp"

#include <yolo.hpp>

CPU6502::CPU6502(uint16_t resetVector)
{
    mYoloModule = yolo::registerModule("CPU", "\e[0;31m");
    this->Reset(resetVector);
}

CPU6502::~CPU6502()
{
}

void CPU6502::Tick()
{

}

void CPU6502::Reset(uint16_t resetVector)
{
    yolo::info(mYoloModule, "6502 Resetting...");
}

