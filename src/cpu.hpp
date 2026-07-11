#pragma once

class CPU6502
{
public:
    CPU6502();
    ~CPU6502();

    void Tick();
    void Reset();

public:
    uint16_t PC;
    uint16_t SP;
    uint8_t Accumilator;
    uint8_t X;
    uint8_t Y;

    union {
        struct {
            uint8_t Carry : 1;
            uint8_t Zero : 1;
            uint8_t IRQDisable : 1;
            uint8_t DecimalMode : 1;
            uint8_t Break : 1;
            uint8_t Overflow : 1;
            uint8_t Negative : 1;
        };
    uint8_t Flags;
    };

private:
    void Fetch();
    void Execute();

    void IRQ();

private:
    
    
};

