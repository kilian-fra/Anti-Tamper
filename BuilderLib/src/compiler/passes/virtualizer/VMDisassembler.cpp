#include <iostream>
#include <vector>
#include <format>
#include "VM.hpp"
#include "VMDisassembler.hpp"

VMDisassembler::VMDisassembler(std::uint8_t* p, std::size_t size)
	: m_PtrBytecode(p), m_Size(size), m_Disassembly("")
{
}

std::string VMDisassembler::fetchInstruction(VM_OPCODE vm_opcode)
{
    switch (vm_opcode) {
        case VM_OPCODE::MOV: return "MOV";
        case VM_OPCODE::MOVBE: return "MOVBE";
        case VM_OPCODE::MOVSX: return "MOVSX";
        case VM_OPCODE::MOVZX: return "MOVZX";
        case VM_OPCODE::MOVD: return "MOVD";
        case VM_OPCODE::MOVNTI: return "MOVNTI";
        case VM_OPCODE::LEA: return "LEA";

        case VM_OPCODE::POP: return "POP";
        case VM_OPCODE::POPA: return "POPA";
        case VM_OPCODE::POPAD: return "POPAD";
        case VM_OPCODE::PUSH: return "PUSH";
        case VM_OPCODE::PUSHA: return "PUSHA";
        case VM_OPCODE::PUSHAD: return "PUSHAD";
        case VM_OPCODE::POPF: return "POPF";
        case VM_OPCODE::POPFD: return "POPFD";
        case VM_OPCODE::POPFQ: return "POPFQ";
        case VM_OPCODE::PUSHF: return "PUSHF";
        case VM_OPCODE::PUSHFD: return "PUSHFD";
        case VM_OPCODE::PUSHFQ: return "PUSHFQ";

        case VM_OPCODE::ADC: return "ADC";
        case VM_OPCODE::ADD: return "ADD";
        case VM_OPCODE::SBB: return "SBB";
        case VM_OPCODE::SUB: return "SUB";
        case VM_OPCODE::NEG: return "NEG";
        case VM_OPCODE::MUL: return "MUL";
        case VM_OPCODE::IMUL: return "IMUL";
        case VM_OPCODE::DIV: return "DIV";
        case VM_OPCODE::IDIV: return "IDIV";
        case VM_OPCODE::INC: return "INC";
        case VM_OPCODE::DEC: return "DEC";

        case VM_OPCODE::RCL: return "RCL";
        case VM_OPCODE::RCR: return "RCR";
        case VM_OPCODE::ROL: return "ROL";
        case VM_OPCODE::ROR: return "ROR";
        case VM_OPCODE::SALC: return "SALC";
        case VM_OPCODE::SAR: return "SAR";
        case VM_OPCODE::SHL: return "SHL";
        case VM_OPCODE::SHLD: return "SHLD";
        case VM_OPCODE::SHRD: return "SHRD";
        case VM_OPCODE::SHR: return "SHR";

        case VM_OPCODE::AND: return "AND";
        case VM_OPCODE::OR: return "OR";
        case VM_OPCODE::NOT: return "NOT";
        case VM_OPCODE::XOR: return "XOR";

        case VM_OPCODE::JMP: return "JMP";
        case VM_OPCODE::CALL: return "CALL";
        case VM_OPCODE::RET: return "RET";

        case VM_OPCODE::JO: return "JO";
        case VM_OPCODE::JNO: return "JNO";
        case VM_OPCODE::JB: return "JB";
        case VM_OPCODE::JNB: return "JNB";
        case VM_OPCODE::JZ: return "JZ";
        case VM_OPCODE::JNZ: return "JNZ";
        case VM_OPCODE::JBE: return "JBE";
        case VM_OPCODE::JNBE: return "JNBE";
        case VM_OPCODE::JS: return "JS";
        case VM_OPCODE::JNS: return "JNS";
        case VM_OPCODE::JP: return "JP";
        case VM_OPCODE::JNP: return "JNP";
        case VM_OPCODE::JL: return "JL";
        case VM_OPCODE::JNL: return "JNL";
        case VM_OPCODE::JLE: return "JLE";
        case VM_OPCODE::JNLE: return "JNLE";

        case VM_OPCODE::CMP: return "CMP";
        case VM_OPCODE::TEST: return "TEST";
        case VM_OPCODE::BT: return "BT";
        case VM_OPCODE::BTC: return "BTC";
        case VM_OPCODE::BTR: return "BTR";
        case VM_OPCODE::BTS: return "BTS";

        // Handle unsupported instructions
        case VM_OPCODE::UNSUPPORTED: return "UNSUPPORTED";

        // Default case (unknown instruction)
        default: return "UNKNOWN";
    }
}

std::string VMDisassembler::fetchFirstOperand(std::uint8_t* vip)
{
    auto vm_opcode = static_cast<VM_OPCODE>(*vip);
    auto opCount = getInstructionOpCount(vm_opcode);
    std::string result = "";

    if (!opCount)
        return "";

    return result;
    
    //if (opCount == 1) {
    //    VM_OPCODE_OP1_FLAGS flags = {};
    //    flags.flags = *(vip + 1);

    //    if (flags.isImmediateUsed) {
    //        switch (flags.immSize) {
    //            case VM_IMM_8:
    //                std::format("0x{:x}", static_cast<std::)
    //                break;
    //            case VM_IMM_16: return 4;
    //            case VM_IMM_32: return 6;
    //            case VM_IMM_64: return 10;
    //            default: return 0;
    //        }
    //    }
    //}
}

std::string VMDisassembler::fetchSecondOperand(std::uint8_t* vip)
{
    return std::string();
}

std::size_t VMDisassembler::getInstructionLength(std::uint8_t* vip)
{
    auto vm_opcode = static_cast<VM_OPCODE>(*vip);
    auto operandsCount = getInstructionOpCount(vm_opcode);

    if (!operandsCount)
        return 1;
    
    if (operandsCount == 1) {
        VM_OPCODE_OP1_FLAGS flags = {};
        flags.flags = *(vip + 1);
        
        if (flags.isImmediateUsed) {
            switch (flags.immSize) {
                case VM_IMM_8: return 3;
                case VM_IMM_16: return 4;
                case VM_IMM_32: return 6;
                case VM_IMM_64: return 10;
                default: return 0;
            }
        }

        if (flags.hasDisplacement && flags.hasScaleIndex) return 13;
        else if (flags.hasDisplacement && !flags.hasScaleIndex) return 11;
        else if (!flags.hasDisplacement && flags.hasScaleIndex) return 5;
        else return 3;
    }
    else {
        VM_OPCODE_OP2_FLAGS flags = {};
        flags.flags = *(vip + 1);

        //Opcode + Flags + Register (base for mem or normal reg value)
        std::size_t size = 3;

        if (flags.isImmediateUsed) {
            switch (flags.immSize) {
                case VM_IMM_8:
                    size += 1;
                    break;
                case VM_IMM_16:
                    size += 2;
                    break;
                case VM_IMM_32:
                    size += 4;
                    break;
                case VM_IMM_64:
                    size += 8;
                    break;
                default: break;
            }
        }
        else {
            size += 1;
        }
 
        if (flags.hasDisplacement && flags.hasScaleIndex) size += 10;
        else if (flags.hasDisplacement && !flags.hasScaleIndex) size += 8;
        else if (!flags.hasDisplacement && flags.hasScaleIndex) size += 2;
        
        return size;
    }
}

std::uint8_t VMDisassembler::getInstructionOpCount(VM_OPCODE vm_opcode)
{
    switch (vm_opcode) {
        // No operands
        case VM_OPCODE::RET:
            return 0;

        //1 Operand
        case VM_OPCODE::NOT:
        case VM_OPCODE::JMP:
        case VM_OPCODE::CALL:
        case VM_OPCODE::JO:
        case VM_OPCODE::JNO:
        case VM_OPCODE::JB:
        case VM_OPCODE::JNB:
        case VM_OPCODE::JZ:
        case VM_OPCODE::JNZ:
        case VM_OPCODE::JBE:
        case VM_OPCODE::JNBE:
        case VM_OPCODE::JS:
        case VM_OPCODE::JNS:
        case VM_OPCODE::JP:
        case VM_OPCODE::JNP:
        case VM_OPCODE::JL:
        case VM_OPCODE::JNL:
        case VM_OPCODE::JLE:
        case VM_OPCODE::JNLE:
        case VM_OPCODE::INC:
        case VM_OPCODE::DEC:
        case VM_OPCODE::POP:
        case VM_OPCODE::POPA:
        case VM_OPCODE::POPAD:
        case VM_OPCODE::PUSH:
        case VM_OPCODE::PUSHA:
        case VM_OPCODE::PUSHAD:
        case VM_OPCODE::POPF:
        case VM_OPCODE::POPFD:
        case VM_OPCODE::POPFQ:
        case VM_OPCODE::PUSHF:
        case VM_OPCODE::PUSHFD:
        case VM_OPCODE::PUSHFQ:
            return 1;

        //2 operands
        case VM_OPCODE::CMP:
        case VM_OPCODE::TEST:
        case VM_OPCODE::BT:
        case VM_OPCODE::BTC:
        case VM_OPCODE::BTR:
        case VM_OPCODE::BTS:
        case VM_OPCODE::AND:
        case VM_OPCODE::OR:
        case VM_OPCODE::XOR:
        case VM_OPCODE::MOV:
        case VM_OPCODE::MOVBE:
        case VM_OPCODE::MOVSX:
        case VM_OPCODE::MOVZX:
        case VM_OPCODE::MOVD:
        case VM_OPCODE::MOVNTI:
        case VM_OPCODE::LEA:
        case VM_OPCODE::ADC:
        case VM_OPCODE::ADD:
        case VM_OPCODE::SBB:
        case VM_OPCODE::SUB:
        case VM_OPCODE::NEG:
        case VM_OPCODE::MUL:
        case VM_OPCODE::IMUL:
        case VM_OPCODE::DIV:
        case VM_OPCODE::IDIV:
        case VM_OPCODE::RCL:
        case VM_OPCODE::RCR:
        case VM_OPCODE::ROL:
        case VM_OPCODE::ROR:
        case VM_OPCODE::SALC:
        case VM_OPCODE::SAR:
        case VM_OPCODE::SHL:
        case VM_OPCODE::SHLD:
        case VM_OPCODE::SHRD:
        case VM_OPCODE::SHR:
            return 2;

        // Handle unsupported instructions
        case VM_OPCODE::UNSUPPORTED: return 0;

        // Default case (unknown instruction)
        default: return 0;
    }
}

void VMDisassembler::disassemble()
{
    auto vip = m_PtrBytecode;
    while (vip < (m_PtrBytecode + m_Size)) {

        //Fetch current instruction
        m_Disassembly += fetchInstruction(static_cast<VM_OPCODE>(*vip));
        m_Disassembly += "\n";

        //Go to next instruction
        vip += getInstructionLength(vip);
    }
}