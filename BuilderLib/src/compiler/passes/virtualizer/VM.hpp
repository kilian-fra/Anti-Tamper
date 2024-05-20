#pragma once

#include <iostream>

/*
union {
        std::uint64_t rax;
        struct {
            union {
                std::uint32_t eax;
                struct {
                    union {
                        std::uint16_t ax;
                        struct {
                            std::uint8_t al;
                            std::uint8_t ah;
                        };
                    };
                };
            };
        };
    };
 */

/*
Constants/Defines
*/

/*
Encode different immediate value sizes for opcode flags
*/
constexpr auto VM_IMM_8 = 0b00U;
constexpr auto VM_IMM_16 = 0b01U;
constexpr auto VM_IMM_32 = 0b10U;
constexpr auto VM_IMM_64 = 0b11U;

/*
* Virtual set of general purpose registers (GPRs)
*/
typedef struct {
    std::uint64_t rax;
    std::uint64_t rbx;
    std::uint64_t rcx;
    std::uint64_t rdx;
    std::uint64_t rdi;
    std::uint64_t rsi;
    std::uint64_t rbp;
    std::uint64_t rsp;
    std::uint64_t r8;
    std::uint64_t r9;
    std::uint64_t r10;
    std::uint64_t r11;
    std::uint64_t r12;
    std::uint64_t r13;
    std::uint64_t r14;
    std::uint64_t r15;
}VGPR, *PVGPR;

/*
Virtual register set
*/
typedef struct {
    VGPR gprs;
    std::uint64_t vip; //virtual instruction pointer (rip)
    std::uint64_t vflags; ///virtual flags register
}VRSET, *PVRSET;

/*
The context of the vm described by the VM_CONTEXT struct.
By entering the VM, the nativ context is backed up and restored after vm exit.
The VM updates the the VM_CONTEXT for every operation.
*/
typedef struct {
    VRSET vrset; //virtual register set

}VM_CONTEXT, *PVM_CONTEXT;

enum class VM_OPCODE : std::uint8_t {
    MOV,
    MOVBE,
    MOVSX,
    MOVZX,
    MOVD,
    MOVNTI,
    LEA,

    /*
    Stack operations
    */
    POP,
    POPA,
    POPAD,
    PUSH,
    PUSHA,
    PUSHAD,

    /*Push and Pop Flags*/
    POPF,
    POPFD,
    POPFQ,
    PUSHF,
    PUSHFD,
    PUSHFQ,

    /*
    Data Conversion
    */

    /*
    General Arithmetic
    */
    ADC,
    ADD,
    SBB,
    SUB,
    NEG,
    MUL,
    IMUL,
    DIV,
    IDIV,
    INC,
    DEC,

    /*
    Rotate and Shift
    */
    RCL,
    RCR,
    ROL,
    ROR,
    SALC,
    SAR,
    SHL,
    SHR,
    SHLD,
    SHRD,

    /*
    Compare and Test
    */
    CMP,
    TEST,
    BT,
    BTC,
    BTR,
    BTS,
    //...

    /*
    Logical operations (only AND, OR & NOT are needed)
    XOR = (A AND NOT B) OR (NOT A AND B)
    */
    AND,
    OR,
    NOT,
    XOR,

    /*
    Control Transfer
    */

    //Jump instructions
    JMP,
    JO,
    JNO,
    JB,
    JNB,
    JZ,
    JNZ,
    JBE,
    JNBE,
    JS,
    JNS,
    JP,
    JNP,
    JL,
    JNL,
    JLE,
    JNLE,
    
    CALL,
    RET,

    /*System Calls*/
    SYSENTER,
    SYSEXIT,
    SYSCALL,
    SYSRET,

    UNSUPPORTED
};

// Define an enum to represent possible operands (registers, etc.)
enum class VM_REG_OPERAND : std::uint8_t {
    // 8-Bit Registers
    REGISTER_AL,
    REGISTER_AH, // Higher 8 bits of AX
    REGISTER_BL,
    REGISTER_BH, // Higher 8 bits of BX
    REGISTER_CL,
    REGISTER_CH, // Higher 8 bits of CX
    REGISTER_DL,
    REGISTER_DH, // Higher 8 bits of DX
    REGISTER_SPL,
    REGISTER_BPL,
    REGISTER_SIL,
    REGISTER_DIL,
    REGISTER_R8B,
    REGISTER_R9B,
    REGISTER_R10B,
    REGISTER_R11B,
    REGISTER_R12B,
    REGISTER_R13B,
    REGISTER_R14B,
    REGISTER_R15B,

    // 16-Bit Registers
    REGISTER_AX,
    REGISTER_BX,
    REGISTER_CX,
    REGISTER_DX,
    REGISTER_SP,
    REGISTER_BP,
    REGISTER_SI,
    REGISTER_DI,
    REGISTER_R8W,
    REGISTER_R9W,
    REGISTER_R10W,
    REGISTER_R11W,
    REGISTER_R12W,
    REGISTER_R13W,
    REGISTER_R14W,
    REGISTER_R15W,

    // 32-Bit Registers
    REGISTER_EAX,
    REGISTER_EBX,
    REGISTER_ECX,
    REGISTER_EDX,
    REGISTER_ESP,
    REGISTER_EBP,
    REGISTER_ESI,
    REGISTER_EDI,
    REGISTER_R8D,
    REGISTER_R9D,
    REGISTER_R10D,
    REGISTER_R11D,
    REGISTER_R12D,
    REGISTER_R13D,
    REGISTER_R14D,
    REGISTER_R15D,

    // 64-Bit Registers
    REGISTER_RAX,
    REGISTER_RBX,
    REGISTER_RCX,
    REGISTER_RDX,
    REGISTER_RSP,
    REGISTER_RBP,
    REGISTER_RSI,
    REGISTER_RDI,
    REGISTER_R8,
    REGISTER_R9,
    REGISTER_R10,
    REGISTER_R11,
    REGISTER_R12,
    REGISTER_R13,
    REGISTER_R14,
    REGISTER_R15,

    REGISTER_RIP, //Needed for RIP-relative addressing
    UNKNOWN
};

typedef union {
    std::uint8_t flags;
    struct {
        std::uint8_t isIndirectOp1 : 1;
        std::uint8_t isIndirectOp2 : 1;
        std::uint8_t isImmediateUsed : 1;
        std::uint8_t isImmSigned : 1;
        std::uint8_t immSize : 2;
        std::uint8_t hasDisplacement : 1;
        std::uint8_t hasScaleIndex : 1;
    };
}VM_OPCODE_OP2_FLAGS;

typedef union {
    std::uint8_t flags;
    struct {
        std::uint8_t isIndirectOp1 : 1;
        std::uint8_t isImmediateUsed : 1;
        std::uint8_t isImmRipRelative : 1;
        std::uint8_t isImmSigned : 1;
        std::uint8_t immSize : 2;
        std::uint8_t hasDisplacement : 1;
        std::uint8_t hasScaleIndex : 1;
    };
}VM_OPCODE_OP1_FLAGS;

//#pragma pack(push, 1)
//typedef struct {
//    VM_OPCODE opcode;
//    VM_OPCODE_FLAGS flags;
//    VM_OPERAND op1;
//}VM_INSTRUCTION_OP1, *PVM_INSTRUCTION_OP1;
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//typedef struct {
//    VM_OPCODE opcode;
//    VM_OPCODE_FLAGS flags;
//    std::uint8_t imm8;
//}VM_INSTRUCTION_OP1_IMM8, * PVM_INSTRUCTION_OP1_IMM8;
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//typedef struct {
//    VM_OPCODE opcode;
//    VM_OPCODE_FLAGS flags;
//    std::uint16_t imm16;
//}VM_INSTRUCTION_OP1_IMM16, * PVM_INSTRUCTION_OP1_IMM16;
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//typedef struct {
//    VM_OPCODE opcode;
//    VM_OPCODE_FLAGS flags;
//    std::uint32_t imm32;
//}VM_INSTRUCTION_OP1_IMM32, * PVM_INSTRUCTION_OP1_IMM32;
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//typedef struct {
//    VM_OPCODE opcode;
//    VM_OPCODE_FLAGS flags;
//    std::uint64_t imm64;
//}VM_INSTRUCTION_OP1_IMM64, * PVM_INSTRUCTION_OP1_IMM64;
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//typedef struct {
//    VM_OPCODE opcode;
//    VM_OPCODE_FLAGS flags;
//    VM_OPERAND op1;
//    VM_OPERAND op2;
//}VM_INSTRUCTION_OP2, * PVM_INSTRUCTION_OP2;
//#pragma pack(pop)
//
//
//
//#pragma pack(push, 1)
//typedef struct {
//    VM_OPCODE opcode;
//    VM_OPCODE_FLAGS flags;
//    VM_OPERAND op1;
//    std::uint8_t imm8;
//}VM_INSTRUCTION_OP2_IMM8, * PVM_INSTRUCTION_OP2_IMM8;
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//typedef struct {
//    VM_OPCODE opcode;
//    VM_OPCODE_FLAGS flags;
//    VM_OPERAND op1;
//    std::uint16_t imm16;
//}VM_INSTRUCTION_OP2_IMM16, * PVM_INSTRUCTION_OP2_IMM16;
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//typedef struct {
//    VM_OPCODE opcode;
//    VM_OPCODE_FLAGS flags;
//    VM_OPERAND op1;
//    std::uint32_t imm32;
//}VM_INSTRUCTION_OP2_IMM32, * PVM_INSTRUCTION_OP2_IMM32;
//#pragma pack(pop)
//
//#pragma pack(push, 1)
//typedef struct {
//    VM_OPCODE opcode;
//    VM_OPCODE_FLAGS flags;
//    VM_OPERAND op1;
//    std::uint64_t imm64;
//}VM_INSTRUCTION_OP2_IMM64, * PVM_INSTRUCTION_OP2_IMM64;
//#pragma pack(pop)