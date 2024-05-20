#include "VMInstruction.hpp"

VMInstruction::VMInstruction(VMContext* pVmContext, VM_OPCODE vmOpcode)
	: m_pVmContext(pVmContext), m_VmOpcode(vmOpcode)
{}

void VMInstruction::decodeOp2(VM_INSTRUCTION_OP2* pInstructionInfoOut) {

    if (!pInstructionInfoOut) return;
    const auto flags = getOp2Flags();

    //Reg. to Reg (direct addressing).
    if (!flags.isImmediateUsed && !flags.isIndirectOp1 && !flags.isIndirectOp2) { //reg, reg
        pInstructionInfoOut->pOp1 = getRegOperandPtr(getRegOp1(), &pInstructionInfoOut->op1SizeBytes);
        pInstructionInfoOut->pOp2 = getRegOperandPtr(getRegOp2(), &pInstructionInfoOut->op2SizeBytes);
    }
    else if (flags.isImmediateUsed && !flags.isIndirectOp1) { //reg, imm
        pInstructionInfoOut->pOp1 = getRegOperandPtr(getRegOp1(), &pInstructionInfoOut->op1SizeBytes);
        pInstructionInfoOut->pOp2 = getImmOp2();
        pInstructionInfoOut->op2SizeBytes = getImmSizeBytes(flags.immSize);
    }
    else if (flags.isIndirectOp1 && !flags.isImmediateUsed) { //indirect, reg
        //mov [base + disp + index * scale], rax
        pInstructionInfoOut->pOp1 = handleIndirectOp();
        pInstructionInfoOut->pOp2 = getRegOperandPtr(getRegOp2(), &pInstructionInfoOut->op2SizeBytes);
        pInstructionInfoOut->op1SizeBytes = pInstructionInfoOut->op2SizeBytes;
    }
    else if (flags.isIndirectOp2 && !flags.isImmediateUsed) { //reg, indirect
        pInstructionInfoOut->pOp1 = getRegOperandPtr(getRegOp2(), &pInstructionInfoOut->op1SizeBytes);
        pInstructionInfoOut->pOp2 = handleIndirectOp();
        pInstructionInfoOut->op2SizeBytes = pInstructionInfoOut->op1SizeBytes;
    }
    else { //indirect, imm
        pInstructionInfoOut->pOp1 = handleIndirectOp();
        pInstructionInfoOut->pOp2 = getImmOp2();
        pInstructionInfoOut->op2SizeBytes = getImmSizeBytes(flags.immSize);
        pInstructionInfoOut->op1SizeBytes = pInstructionInfoOut->op2SizeBytes;
    }
}

void VMInstruction::decodeOp1(VM_INSTRUCTION_OP1* pInstructionInfoOut) {
    const auto flags = getOp1Flags();

    if (!flags.isIndirectOp1 && !flags.isImmediateUsed) { //reg
        pInstructionInfoOut->pOp1 = getRegOperandPtr(getRegOp1(), &pInstructionInfoOut->op1SizeBytes);
    }
    else if (flags.isIndirectOp1 && !flags.isImmediateUsed) { //indirect
        pInstructionInfoOut->pOp1 = handleIndirectOp();
    }
    else { //imm
        pInstructionInfoOut->pOp1 = getImmOp1();
        pInstructionInfoOut->op1SizeBytes = getImmSizeBytes(flags.immSize);
    }
}

void* VMInstruction::handleIndirectOp()
{
    std::uint8_t* pMemLoc = nullptr;
    const auto opCount = getOpCount();

    if (!opCount) return nullptr;

    VM_REG_OPERAND base = VM_REG_OPERAND::UNKNOWN;
    std::uint8_t hasDisplacement = 0;
    std::uint8_t hasScaleIndex = 0;
    std::uint8_t dispOffset = 0;
    std::uint8_t scaleIndexOffset = 0;

    if (opCount == 1) {
        auto flags = getOp1Flags();
        if (!flags.isIndirectOp1) return nullptr;
        base = getRegOp1();
        hasDisplacement = flags.hasDisplacement;
        hasScaleIndex = flags.hasScaleIndex;

        dispOffset = 3;
        scaleIndexOffset = dispOffset + 8;
    }
    else {
        auto flags = getOp2Flags();
        if (!flags.isIndirectOp1 && !flags.isIndirectOp2) return nullptr;
        base = flags.isIndirectOp1 ? getRegOp1() : getRegOp2();
        hasDisplacement = flags.hasDisplacement;
        hasScaleIndex = flags.hasScaleIndex;

        dispOffset = flags.isImmediateUsed ? 3 + getImmSizeBytes(flags.immSize) : 4;
        scaleIndexOffset = dispOffset + 8;
    }

    if (base == VM_REG_OPERAND::REGISTER_RIP) {
        pMemLoc = m_pVmContext->getPtrRealFunc();
    }
    else if (base != VM_REG_OPERAND::UNKNOWN) {
        std::uint8_t regSize = 0;
        pMemLoc = (std::uint8_t*)(*reinterpret_cast<std::uint64_t*>(getRegOperandPtr(base, &regSize)));
    }

    //Get disp (if available)
    if (hasDisplacement)
        pMemLoc += *reinterpret_cast<std::int64_t*>(m_pVmContext->getVip() + dispOffset);

    //Get scale and index (if available)
    std::uint8_t scale = 0;
    VM_REG_OPERAND index = VM_REG_OPERAND::UNKNOWN;
    if (hasScaleIndex) {
        scale = *reinterpret_cast<std::uint8_t*>(m_pVmContext->getVip() + scaleIndexOffset);
        std::uint8_t indexSize = 0;
        auto pIndex = getRegOperandPtr(*reinterpret_cast<VM_REG_OPERAND*>
            (m_pVmContext->getVip() + scaleIndexOffset + 1), &indexSize);
        switch (indexSize) {
            case 1: 
                pMemLoc += static_cast<std::uint64_t>((*reinterpret_cast<std::uint8_t*>(pIndex) * scale));
                break;
            case 2:
                pMemLoc += static_cast<std::uint64_t>((*reinterpret_cast<std::uint16_t*>(pIndex) * scale));
                break;
            case 4:
                pMemLoc += static_cast<std::uint64_t>((*reinterpret_cast<std::uint32_t*>(pIndex) * scale));
                break;
            case 8:
                pMemLoc += static_cast<std::uint64_t>((*reinterpret_cast<std::uint64_t*>(pIndex) * scale));
                break;
        }
    }

    return pMemLoc;
}

std::uint8_t VMInstruction::getOpCount() const
{
    switch (m_VmOpcode) {
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
    case VM_OPCODE::PUSH:
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

void* VMInstruction::getRegOperandPtr(VM_REG_OPERAND regOperand, std::uint8_t* regSize) {
    switch (regOperand) {
        // 64-Bit Registers
    case VM_REG_OPERAND::REGISTER_RAX:
        *regSize = sizeof(m_pVmContext->m_Gpr.rax);
        return &m_pVmContext->m_Gpr.rax;

    case VM_REG_OPERAND::REGISTER_RBX:
        *regSize = sizeof(m_pVmContext->m_Gpr.rbx);
        return &m_pVmContext->m_Gpr.rbx;

    case VM_REG_OPERAND::REGISTER_RCX:
        *regSize = sizeof(m_pVmContext->m_Gpr.rcx);
        return &m_pVmContext->m_Gpr.rcx;

    case VM_REG_OPERAND::REGISTER_RDX:
        *regSize = sizeof(m_pVmContext->m_Gpr.rdx);
        return &m_pVmContext->m_Gpr.rdx;

    case VM_REG_OPERAND::REGISTER_RDI:
        *regSize = sizeof(m_pVmContext->m_Gpr.rdi);
        return &m_pVmContext->m_Gpr.rdi;

    case VM_REG_OPERAND::REGISTER_RSI:
        *regSize = sizeof(m_pVmContext->m_Gpr.rsi);
        return &m_pVmContext->m_Gpr.rsi;

    case VM_REG_OPERAND::REGISTER_RBP:
        *regSize = sizeof(m_pVmContext->m_Gpr.rbp);
        return &m_pVmContext->m_Gpr.rbp;

    case VM_REG_OPERAND::REGISTER_RSP:
        *regSize = sizeof(m_pVmContext->m_Gpr.rsp);
        return &m_pVmContext->m_Gpr.rsp;

    case VM_REG_OPERAND::REGISTER_R8:
        *regSize = sizeof(m_pVmContext->m_Gpr.r8);
        return &m_pVmContext->m_Gpr.r8;

    case VM_REG_OPERAND::REGISTER_R9:
        *regSize = sizeof(m_pVmContext->m_Gpr.r9);
        return &m_pVmContext->m_Gpr.r9;

    case VM_REG_OPERAND::REGISTER_R10:
        *regSize = sizeof(m_pVmContext->m_Gpr.r10);
        return &m_pVmContext->m_Gpr.r10;

    case VM_REG_OPERAND::REGISTER_R11:
        *regSize = sizeof(m_pVmContext->m_Gpr.r11);
        return &m_pVmContext->m_Gpr.r11;

    case VM_REG_OPERAND::REGISTER_R12:
        *regSize = sizeof(m_pVmContext->m_Gpr.r12);
        return &m_pVmContext->m_Gpr.r12;

    case VM_REG_OPERAND::REGISTER_R13:
        *regSize = sizeof(m_pVmContext->m_Gpr.r13);
        return &m_pVmContext->m_Gpr.r13;

    case VM_REG_OPERAND::REGISTER_R14:
        *regSize = sizeof(m_pVmContext->m_Gpr.r14);
        return &m_pVmContext->m_Gpr.r14;

    case VM_REG_OPERAND::REGISTER_R15:
        *regSize = sizeof(m_pVmContext->m_Gpr.r15);
        return &m_pVmContext->m_Gpr.r15;

        // 32-Bit Registers (lower 32 bits)
    case VM_REG_OPERAND::REGISTER_EAX:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.rax);

    case VM_REG_OPERAND::REGISTER_EBX:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.rbx);

    case VM_REG_OPERAND::REGISTER_ECX:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.rcx);

    case VM_REG_OPERAND::REGISTER_EDX:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.rdx);

    case VM_REG_OPERAND::REGISTER_ESP:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.rsp);

    case VM_REG_OPERAND::REGISTER_EBP:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.rbp);

    case VM_REG_OPERAND::REGISTER_ESI:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.rsi);

    case VM_REG_OPERAND::REGISTER_EDI:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.rdi);

    case VM_REG_OPERAND::REGISTER_R8D:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.r8);

    case VM_REG_OPERAND::REGISTER_R9D:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.r9);

    case VM_REG_OPERAND::REGISTER_R10D:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.r10);

    case VM_REG_OPERAND::REGISTER_R11D:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.r11);

    case VM_REG_OPERAND::REGISTER_R12D:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.r12);

    case VM_REG_OPERAND::REGISTER_R13D:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.r13);

    case VM_REG_OPERAND::REGISTER_R14D:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.r14);

    case VM_REG_OPERAND::REGISTER_R15D:
        *regSize = sizeof(std::uint32_t);
        return reinterpret_cast<std::uint32_t*>(&m_pVmContext->m_Gpr.r15);

        // 16-Bit Registers
    case VM_REG_OPERAND::REGISTER_AX:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.rax);

    case VM_REG_OPERAND::REGISTER_BX:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.rbx);

    case VM_REG_OPERAND::REGISTER_CX:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.rcx);

    case VM_REG_OPERAND::REGISTER_DX:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.rdx);

    case VM_REG_OPERAND::REGISTER_SP:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.rsp);

    case VM_REG_OPERAND::REGISTER_BP:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.rbp);

    case VM_REG_OPERAND::REGISTER_SI:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.rsi);

    case VM_REG_OPERAND::REGISTER_DI:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.rdi);

    case VM_REG_OPERAND::REGISTER_R8W:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.r8);

    case VM_REG_OPERAND::REGISTER_R9W:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.r9);

    case VM_REG_OPERAND::REGISTER_R10W:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.r10);

    case VM_REG_OPERAND::REGISTER_R11W:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.r11);

    case VM_REG_OPERAND::REGISTER_R12W:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.r12);

    case VM_REG_OPERAND::REGISTER_R13W:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.r13);

    case VM_REG_OPERAND::REGISTER_R14W:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.r14);

    case VM_REG_OPERAND::REGISTER_R15W:
        *regSize = sizeof(std::uint16_t);
        return reinterpret_cast<std::uint16_t*>(&m_pVmContext->m_Gpr.r15);

        // 8-Bit Registers
    case VM_REG_OPERAND::REGISTER_AL:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rax);

    case VM_REG_OPERAND::REGISTER_AH:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rax) + 1;

    case VM_REG_OPERAND::REGISTER_BL:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rbx);

    case VM_REG_OPERAND::REGISTER_BH:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rbx) + 1;

    case VM_REG_OPERAND::REGISTER_CL:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rcx);

    case VM_REG_OPERAND::REGISTER_CH:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rcx) + 1;

    case VM_REG_OPERAND::REGISTER_DL:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rdx);

    case VM_REG_OPERAND::REGISTER_DH:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rdx) + 1;

    case VM_REG_OPERAND::REGISTER_SPL:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rsp);

    case VM_REG_OPERAND::REGISTER_BPL:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rbp);

    case VM_REG_OPERAND::REGISTER_SIL:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rsi);

    case VM_REG_OPERAND::REGISTER_DIL:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.rdi);

    case VM_REG_OPERAND::REGISTER_R8B:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.r8);

    case VM_REG_OPERAND::REGISTER_R9B:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.r9);

    case VM_REG_OPERAND::REGISTER_R10B:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.r10);

    case VM_REG_OPERAND::REGISTER_R11B:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.r11);

    case VM_REG_OPERAND::REGISTER_R12B:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.r12);

    case VM_REG_OPERAND::REGISTER_R13B:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.r13);

    case VM_REG_OPERAND::REGISTER_R14B:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.r14);

    case VM_REG_OPERAND::REGISTER_R15B:
        *regSize = sizeof(std::uint8_t);
        return reinterpret_cast<std::uint8_t*>(&m_pVmContext->m_Gpr.r15);

        // Handle other cases if needed

    default:
        return nullptr; // Unknown register
    }
}