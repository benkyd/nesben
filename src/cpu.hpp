#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

class CPU6502
{
public:
    CPU6502(uint16_t resetVector = 0x000);
    ~CPU6502();

    void Tick();
    void Reset(uint16_t resetVector = 0x000);

public:
    uint16_t PC;
    uint8_t SP;
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
            uint8_t Unused : 1;
            uint8_t Overflow : 1;
            uint8_t Negative : 1;
        };
    uint8_t Flags;
    };

private:
    void Fetch();
    void Decode();
    void Execute();

    void IRQ();
    void NMI();

private:
    void mA_ACC(); void mA_ABS(); void mA_ABSX(); void mA_ABSY(); void mA_IMM();
    void mA_IMP(); void mA_IND(); void mA_INDX(); void mA_INDY(); void mA_REL();
    void mA_ZPG();                void mA_ZPGX(); void mA_ZPGY();

    void mI_ADC(); void mI_AND(); void mI_ASL(); void mI_BCC(); void mI_BCS();
    void mI_BEQ(); void mI_BIT(); void mI_BMI(); void mI_BNE(); void mI_BPL();
    void mI_BRK(); void mI_BVC(); void mI_BVS(); void mI_CLC(); void mI_CLD();
    void mI_CLI(); void mI_CLV(); void mI_CMP(); void mI_CPX(); void mI_CPY();
    void mI_DEC(); void mI_DEX(); void mI_DEY(); void mI_EOR(); void mI_INC();
    void mI_INX(); void mI_INY(); void mI_JMP(); void mI_JSR(); void mI_LDA();
    void mI_LDX(); void mI_LDY(); void mI_LSR(); void mI_NOP(); void mI_ORA();
    void mI_PHA(); void mI_PHP(); void mI_PLA(); void mI_PLP(); void mI_ROL();
    void mI_ROR(); void mI_RTI(); void mI_RTS(); void mI_SBC(); void mI_SEC();
    void mI_SED(); void mI_SEI(); void mI_STA(); void mI_STX(); void mI_STY();
    void mI_TAX(); void mI_TAY(); void mI_TSX(); void mI_TXA(); void mI_TXS();
    void mI_TYA();

    using AddrModeFn = void (CPU6502::*)();
    using OperationFn = void (CPU6502::*)();

    typedef struct EncodedInstruction
    {
        const char*  Name;
        uint8_t      Opcode;
        uint8_t      BaseCycles;   // total cycles, before branch/page-cross
        uint8_t      EffectCycle;  // which cycle does the real R/W + apply Op
        AddrModeFn   AddressingMode;
        OperationFn  Operation;
    } EncodedInstruction_t;

    static const std::unordered_map<uint8_t, EncodedInstruction_t> mEncoderTable
    {
        // ---- Implied / Accumulator (2 cycles) ----
        {0x18,{"CLC",0x18,2,2,&CPU6502::mA_IMP,&CPU6502::mI_CLC}},
        {0xD8,{"CLD",0xD8,2,2,&CPU6502::mA_IMP,&CPU6502::mI_CLD}},
        {0x58,{"CLI",0x58,2,2,&CPU6502::mA_IMP,&CPU6502::mI_CLI}},
        {0xB8,{"CLV",0xB8,2,2,&CPU6502::mA_IMP,&CPU6502::mI_CLV}},
        {0x38,{"SEC",0x38,2,2,&CPU6502::mA_IMP,&CPU6502::mI_SEC}},
        {0xF8,{"SED",0xF8,2,2,&CPU6502::mA_IMP,&CPU6502::mI_SED}},
        {0x78,{"SEI",0x78,2,2,&CPU6502::mA_IMP,&CPU6502::mI_SEI}},
        {0xAA,{"TAX",0xAA,2,2,&CPU6502::mA_IMP,&CPU6502::mI_TAX}},
        {0xA8,{"TAY",0xA8,2,2,&CPU6502::mA_IMP,&CPU6502::mI_TAY}},
        {0xBA,{"TSX",0xBA,2,2,&CPU6502::mA_IMP,&CPU6502::mI_TSX}},
        {0x8A,{"TXA",0x8A,2,2,&CPU6502::mA_IMP,&CPU6502::mI_TXA}},
        {0x9A,{"TXS",0x9A,2,2,&CPU6502::mA_IMP,&CPU6502::mI_TXS}},
        {0x98,{"TYA",0x98,2,2,&CPU6502::mA_IMP,&CPU6502::mI_TYA}},
        {0xEA,{"NOP",0xEA,2,2,&CPU6502::mA_IMP,&CPU6502::mI_NOP}},
        {0xCA,{"DEX",0xCA,2,2,&CPU6502::mA_IMP,&CPU6502::mI_DEX}},
        {0x88,{"DEY",0x88,2,2,&CPU6502::mA_IMP,&CPU6502::mI_DEY}},
        {0xE8,{"INX",0xE8,2,2,&CPU6502::mA_IMP,&CPU6502::mI_INX}},
        {0xC8,{"INY",0xC8,2,2,&CPU6502::mA_IMP,&CPU6502::mI_INY}},
        {0x0A,{"ASL",0x0A,2,2,&CPU6502::mA_ACC,&CPU6502::mI_ASL}},
        {0x4A,{"LSR",0x4A,2,2,&CPU6502::mA_ACC,&CPU6502::mI_LSR}},
        {0x2A,{"ROL",0x2A,2,2,&CPU6502::mA_ACC,&CPU6502::mI_ROL}},
        {0x6A,{"ROR",0x6A,2,2,&CPU6502::mA_ACC,&CPU6502::mI_ROR}},

        // ---- Stack ----
        {0x48,{"PHA",0x48,3,3,&CPU6502::mA_IMP,&CPU6502::mI_PHA}},
        {0x08,{"PHP",0x08,3,3,&CPU6502::mA_IMP,&CPU6502::mI_PHP}},
        {0x68,{"PLA",0x68,4,4,&CPU6502::mA_IMP,&CPU6502::mI_PLA}},
        {0x28,{"PLP",0x28,4,4,&CPU6502::mA_IMP,&CPU6502::mI_PLP}},

        // ---- Immediate ----
        {0x69,{"ADC",0x69,2,2,&CPU6502::mA_IMM,&CPU6502::mI_ADC}},
        {0x29,{"AND",0x29,2,2,&CPU6502::mA_IMM,&CPU6502::mI_AND}},
        {0xC9,{"CMP",0xC9,2,2,&CPU6502::mA_IMM,&CPU6502::mI_CMP}},
        {0xE0,{"CPX",0xE0,2,2,&CPU6502::mA_IMM,&CPU6502::mI_CPX}},
        {0xC0,{"CPY",0xC0,2,2,&CPU6502::mA_IMM,&CPU6502::mI_CPY}},
        {0x49,{"EOR",0x49,2,2,&CPU6502::mA_IMM,&CPU6502::mI_EOR}},
        {0xA9,{"LDA",0xA9,2,2,&CPU6502::mA_IMM,&CPU6502::mI_LDA}},
        {0xA2,{"LDX",0xA2,2,2,&CPU6502::mA_IMM,&CPU6502::mI_LDX}},
        {0xA0,{"LDY",0xA0,2,2,&CPU6502::mA_IMM,&CPU6502::mI_LDY}},
        {0x09,{"ORA",0x09,2,2,&CPU6502::mA_IMM,&CPU6502::mI_ORA}},
        {0xE9,{"SBC",0xE9,2,2,&CPU6502::mA_IMM,&CPU6502::mI_SBC}},

        // ---- Zero Page ----
        {0x65,{"ADC",0x65,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_ADC}},
        {0x25,{"AND",0x25,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_AND}},
        {0xC5,{"CMP",0xC5,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_CMP}},
        {0xE4,{"CPX",0xE4,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_CPX}},
        {0xC4,{"CPY",0xC4,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_CPY}},
        {0x45,{"EOR",0x45,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_EOR}},
        {0xA5,{"LDA",0xA5,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_LDA}},
        {0xA6,{"LDX",0xA6,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_LDX}},
        {0xA4,{"LDY",0xA4,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_LDY}},
        {0x05,{"ORA",0x05,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_ORA}},
        {0xE5,{"SBC",0xE5,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_SBC}},
        {0x85,{"STA",0x85,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_STA}},
        {0x86,{"STX",0x86,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_STX}},
        {0x84,{"STY",0x84,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_STY}},
        {0x24,{"BIT",0x24,3,3,&CPU6502::mA_ZPG,&CPU6502::mI_BIT}},
        {0x06,{"ASL",0x06,5,4,&CPU6502::mA_ZPG,&CPU6502::mI_ASL}},
        {0xC6,{"DEC",0xC6,5,4,&CPU6502::mA_ZPG,&CPU6502::mI_DEC}},
        {0xE6,{"INC",0xE6,5,4,&CPU6502::mA_ZPG,&CPU6502::mI_INC}},
        {0x46,{"LSR",0x46,5,4,&CPU6502::mA_ZPG,&CPU6502::mI_LSR}},
        {0x26,{"ROL",0x26,5,4,&CPU6502::mA_ZPG,&CPU6502::mI_ROL}},
        {0x66,{"ROR",0x66,5,4,&CPU6502::mA_ZPG,&CPU6502::mI_ROR}},

        // ---- Zero Page,X / Zero Page,Y ----
        {0x75,{"ADC",0x75,4,4,&CPU6502::mA_ZPGX,&CPU6502::mI_ADC}},
        {0x35,{"AND",0x35,4,4,&CPU6502::mA_ZPGX,&CPU6502::mI_AND}},
        {0xD5,{"CMP",0xD5,4,4,&CPU6502::mA_ZPGX,&CPU6502::mI_CMP}},
        {0x55,{"EOR",0x55,4,4,&CPU6502::mA_ZPGX,&CPU6502::mI_EOR}},
        {0xB5,{"LDA",0xB5,4,4,&CPU6502::mA_ZPGX,&CPU6502::mI_LDA}},
        {0xB4,{"LDY",0xB4,4,4,&CPU6502::mA_ZPGX,&CPU6502::mI_LDY}},
        {0x15,{"ORA",0x15,4,4,&CPU6502::mA_ZPGX,&CPU6502::mI_ORA}},
        {0xF5,{"SBC",0xF5,4,4,&CPU6502::mA_ZPGX,&CPU6502::mI_SBC}},
        {0x95,{"STA",0x95,4,4,&CPU6502::mA_ZPGX,&CPU6502::mI_STA}},
        {0x94,{"STY",0x94,4,4,&CPU6502::mA_ZPGX,&CPU6502::mI_STY}},
        {0xB6,{"LDX",0xB6,4,4,&CPU6502::mA_ZPGY,&CPU6502::mI_LDX}},
        {0x96,{"STX",0x96,4,4,&CPU6502::mA_ZPGY,&CPU6502::mI_STX}},
        {0x16,{"ASL",0x16,6,5,&CPU6502::mA_ZPGX,&CPU6502::mI_ASL}},
        {0xD6,{"DEC",0xD6,6,5,&CPU6502::mA_ZPGX,&CPU6502::mI_DEC}},
        {0xF6,{"INC",0xF6,6,5,&CPU6502::mA_ZPGX,&CPU6502::mI_INC}},
        {0x56,{"LSR",0x56,6,5,&CPU6502::mA_ZPGX,&CPU6502::mI_LSR}},
        {0x36,{"ROL",0x36,6,5,&CPU6502::mA_ZPGX,&CPU6502::mI_ROL}},
        {0x76,{"ROR",0x76,6,5,&CPU6502::mA_ZPGX,&CPU6502::mI_ROR}},

        // ---- Absolute ----
        {0x6D,{"ADC",0x6D,4,4,&CPU6502::mA_ABS,&CPU6502::mI_ADC}},
        {0x2D,{"AND",0x2D,4,4,&CPU6502::mA_ABS,&CPU6502::mI_AND}},
        {0xCD,{"CMP",0xCD,4,4,&CPU6502::mA_ABS,&CPU6502::mI_CMP}},
        {0xEC,{"CPX",0xEC,4,4,&CPU6502::mA_ABS,&CPU6502::mI_CPX}},
        {0xCC,{"CPY",0xCC,4,4,&CPU6502::mA_ABS,&CPU6502::mI_CPY}},
        {0x4D,{"EOR",0x4D,4,4,&CPU6502::mA_ABS,&CPU6502::mI_EOR}},
        {0xAD,{"LDA",0xAD,4,4,&CPU6502::mA_ABS,&CPU6502::mI_LDA}},
        {0xAE,{"LDX",0xAE,4,4,&CPU6502::mA_ABS,&CPU6502::mI_LDX}},
        {0xAC,{"LDY",0xAC,4,4,&CPU6502::mA_ABS,&CPU6502::mI_LDY}},
        {0x0D,{"ORA",0x0D,4,4,&CPU6502::mA_ABS,&CPU6502::mI_ORA}},
        {0xED,{"SBC",0xED,4,4,&CPU6502::mA_ABS,&CPU6502::mI_SBC}},
        {0x8D,{"STA",0x8D,4,4,&CPU6502::mA_ABS,&CPU6502::mI_STA}},
        {0x8E,{"STX",0x8E,4,4,&CPU6502::mA_ABS,&CPU6502::mI_STX}},
        {0x8C,{"STY",0x8C,4,4,&CPU6502::mA_ABS,&CPU6502::mI_STY}},
        {0x2C,{"BIT",0x2C,4,4,&CPU6502::mA_ABS,&CPU6502::mI_BIT}},
        {0x4C,{"JMP",0x4C,3,3,&CPU6502::mA_ABS,&CPU6502::mI_JMP}},
        {0x20,{"JSR",0x20,6,6,&CPU6502::mA_ABS,&CPU6502::mI_JSR}},
        {0x0E,{"ASL",0x0E,6,5,&CPU6502::mA_ABS,&CPU6502::mI_ASL}},
        {0xCE,{"DEC",0xCE,6,5,&CPU6502::mA_ABS,&CPU6502::mI_DEC}},
        {0xEE,{"INC",0xEE,6,5,&CPU6502::mA_ABS,&CPU6502::mI_INC}},
        {0x4E,{"LSR",0x4E,6,5,&CPU6502::mA_ABS,&CPU6502::mI_LSR}},
        {0x2E,{"ROL",0x2E,6,5,&CPU6502::mA_ABS,&CPU6502::mI_ROL}},
        {0x6E,{"ROR",0x6E,6,5,&CPU6502::mA_ABS,&CPU6502::mI_ROR}},

        // ---- Absolute,X ----
        {0x7D,{"ADC",0x7D,4,4,&CPU6502::mA_ABSX,&CPU6502::mI_ADC}},
        {0x3D,{"AND",0x3D,4,4,&CPU6502::mA_ABSX,&CPU6502::mI_AND}},
        {0xDD,{"CMP",0xDD,4,4,&CPU6502::mA_ABSX,&CPU6502::mI_CMP}},
        {0x5D,{"EOR",0x5D,4,4,&CPU6502::mA_ABSX,&CPU6502::mI_EOR}},
        {0xBD,{"LDA",0xBD,4,4,&CPU6502::mA_ABSX,&CPU6502::mI_LDA}},
        {0xBC,{"LDY",0xBC,4,4,&CPU6502::mA_ABSX,&CPU6502::mI_LDY}},
        {0x1D,{"ORA",0x1D,4,4,&CPU6502::mA_ABSX,&CPU6502::mI_ORA}},
        {0xFD,{"SBC",0xFD,4,4,&CPU6502::mA_ABSX,&CPU6502::mI_SBC}},
        {0x9D,{"STA",0x9D,5,5,&CPU6502::mA_ABSX,&CPU6502::mI_STA}},
        {0x1E,{"ASL",0x1E,7,6,&CPU6502::mA_ABSX,&CPU6502::mI_ASL}},
        {0xDE,{"DEC",0xDE,7,6,&CPU6502::mA_ABSX,&CPU6502::mI_DEC}},
        {0xFE,{"INC",0xFE,7,6,&CPU6502::mA_ABSX,&CPU6502::mI_INC}},
        {0x5E,{"LSR",0x5E,7,6,&CPU6502::mA_ABSX,&CPU6502::mI_LSR}},
        {0x3E,{"ROL",0x3E,7,6,&CPU6502::mA_ABSX,&CPU6502::mI_ROL}},
        {0x7E,{"ROR",0x7E,7,6,&CPU6502::mA_ABSX,&CPU6502::mI_ROR}},

        // ---- Absolute,Y ----
        {0x79,{"ADC",0x79,4,4,&CPU6502::mA_ABSY,&CPU6502::mI_ADC}},
        {0x39,{"AND",0x39,4,4,&CPU6502::mA_ABSY,&CPU6502::mI_AND}},
        {0xD9,{"CMP",0xD9,4,4,&CPU6502::mA_ABSY,&CPU6502::mI_CMP}},
        {0x59,{"EOR",0x59,4,4,&CPU6502::mA_ABSY,&CPU6502::mI_EOR}},
        {0xB9,{"LDA",0xB9,4,4,&CPU6502::mA_ABSY,&CPU6502::mI_LDA}},
        {0xBE,{"LDX",0xBE,4,4,&CPU6502::mA_ABSY,&CPU6502::mI_LDX}},
        {0x19,{"ORA",0x19,4,4,&CPU6502::mA_ABSY,&CPU6502::mI_ORA}},
        {0xF9,{"SBC",0xF9,4,4,&CPU6502::mA_ABSY,&CPU6502::mI_SBC}},
        {0x99,{"STA",0x99,5,5,&CPU6502::mA_ABSY,&CPU6502::mI_STA}},

        // ---- (Indirect,X) ----
        {0x61,{"ADC",0x61,6,6,&CPU6502::mA_INDX,&CPU6502::mI_ADC}},
        {0x21,{"AND",0x21,6,6,&CPU6502::mA_INDX,&CPU6502::mI_AND}},
        {0xC1,{"CMP",0xC1,6,6,&CPU6502::mA_INDX,&CPU6502::mI_CMP}},
        {0x41,{"EOR",0x41,6,6,&CPU6502::mA_INDX,&CPU6502::mI_EOR}},
        {0xA1,{"LDA",0xA1,6,6,&CPU6502::mA_INDX,&CPU6502::mI_LDA}},
        {0x01,{"ORA",0x01,6,6,&CPU6502::mA_INDX,&CPU6502::mI_ORA}},
        {0xE1,{"SBC",0xE1,6,6,&CPU6502::mA_INDX,&CPU6502::mI_SBC}},
        {0x81,{"STA",0x81,6,6,&CPU6502::mA_INDX,&CPU6502::mI_STA}},

        // ---- (Indirect),Y ----
        {0x71,{"ADC",0x71,5,5,&CPU6502::mA_INDY,&CPU6502::mI_ADC}},
        {0x31,{"AND",0x31,5,5,&CPU6502::mA_INDY,&CPU6502::mI_AND}},
        {0xD1,{"CMP",0xD1,5,5,&CPU6502::mA_INDY,&CPU6502::mI_CMP}},
        {0x51,{"EOR",0x51,5,5,&CPU6502::mA_INDY,&CPU6502::mI_EOR}},
        {0xB1,{"LDA",0xB1,5,5,&CPU6502::mA_INDY,&CPU6502::mI_LDA}},
        {0x11,{"ORA",0x11,5,5,&CPU6502::mA_INDY,&CPU6502::mI_ORA}},
        {0xF1,{"SBC",0xF1,5,5,&CPU6502::mA_INDY,&CPU6502::mI_SBC}},
        {0x91,{"STA",0x91,6,6,&CPU6502::mA_INDY,&CPU6502::mI_STA}},

        // ---- Indirect (JMP only) ----
        {0x6C,{"JMP",0x6C,5,5,&CPU6502::mA_IND,&CPU6502::mI_JMP}},

        // ---- Relative (branches) ----
        {0x90,{"BCC",0x90,2,0,&CPU6502::mA_REL,&CPU6502::mI_BCC}},
        {0xB0,{"BCS",0xB0,2,0,&CPU6502::mA_REL,&CPU6502::mI_BCS}},
        {0xF0,{"BEQ",0xF0,2,0,&CPU6502::mA_REL,&CPU6502::mI_BEQ}},
        {0x30,{"BMI",0x30,2,0,&CPU6502::mA_REL,&CPU6502::mI_BMI}},
        {0xD0,{"BNE",0xD0,2,0,&CPU6502::mA_REL,&CPU6502::mI_BNE}},
        {0x10,{"BPL",0x10,2,0,&CPU6502::mA_REL,&CPU6502::mI_BPL}},
        {0x50,{"BVC",0x50,2,0,&CPU6502::mA_REL,&CPU6502::mI_BVC}},
        {0x70,{"BVS",0x70,2,0,&CPU6502::mA_REL,&CPU6502::mI_BVS}},

        // ---- Misc control flow ----
        {0x00,{"BRK",0x00,7,7,&CPU6502::mA_IMP,&CPU6502::mI_BRK}},
        {0x40,{"RTI",0x40,6,6,&CPU6502::mA_IMP,&CPU6502::mI_RTI}},
        {0x60,{"RTS",0x60,6,6,&CPU6502::mA_IMP,&CPU6502::mI_RTS}},
    };

    uint16_t mResetVector;
private:
    uint8_t mYoloModule;
};

